#include "../includes/minishell.h"

int	ft_check_builtin(t_execcmd *ecmd)
{
		if (ft_strncmp(ecmd->argv[0], "pwd", 3) == 0 && ecmd->argv[0][3] == '\0')
			return(builtin_pwd(), 1);
		else if (ft_strncmp(ecmd->argv[0], "echo", 4) == 0 && ecmd->argv[0][4] == '\0')
			return(builtin_echo(ecmd), 1);
		else if (ft_strncmp(ecmd->argv[0], "cd", 2) == 0 && ecmd->argv[0][2] == '\0')
			return(builtin_cd(ecmd), 1);
		else if (ft_strncmp(ecmd->argv[0], "exit", 4) == 0 && ecmd->argv[0][4] == '\0')
			return(builtin_exit(ecmd), 1);
		else
			return(0);
}

int ft_runcmd(t_cmd *cmd, char **envp)
{
	int p[2];
	int status_left;
	int	status_right;
	int	status;
	pid_t	pid;
	pid_t	pid_left;
	pid_t	pid_right;
	int	fd;
	t_execcmd *ecmd;
	t_pipecmd *pcmd;
	t_redircmd *rcmd;
	t_andcmd *acmd;
	t_orcmd *ocmd;
	/* t_heredoccmd *hcmd; */

	if (cmd == NULL)
		return 0;
	else if (cmd->type == EXEC)
	{
		ecmd = (t_execcmd *)cmd;
		if (ecmd->argv[0] == NULL)
			return 0;
		if(ft_check_builtin(ecmd))  //NO NODO BUILTIN, VERIFICO PRIMA DI ECXECVE SE BUILTIN
			return(0);
		pid = fork1();
		if(pid == 0)
		{
			setup_signals_child();
			execve(find_path(ecmd->argv), ecmd->argv, envp);
			printf("exec %s failed\n", ecmd->argv[0]);
			exit(127);
		}
		/* wait(NULL);  //return(checkstatus di pipex!!!!!!) */
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
    		return WEXITSTATUS(status);
		else
    		return 1;
	}
	else if (cmd->type == EXECP)
	{
		ecmd = (t_execcmd *)cmd;
		if (ecmd->argv[0] == NULL)
			return 0;
		if(ft_check_builtin(ecmd))  //NO NODO BUILTIN, VERIFICO PRIMA DI ECXEC SE BUILTIN
			return(0);
		/* if(fork1() == 0)
		{ */
		/* execvp(ecmd->argv[0], ecmd->argv); //OLD VERSION,  NOT ADMITTED FUNCTION */
		execve(find_path(ecmd->argv), ecmd->argv, envp);
		printf("exec %s failed\n", ecmd->argv[0]);
		/* exit(1); */
		/* }
		wait(NULL); */
		exit (127);
	}
	else if (cmd->type == REDIR)	//REDIRECT NEL FIGLIO E NON NEL PADRE!!!
	{
		rcmd = (t_redircmd *)cmd;
		fd = open(rcmd->file, rcmd->mode, 0644);
		/* // 1) Apri il file (con O_CREAT se serve)   //vedi se necessario distinguere i due casi
    	if (rc->mode & O_CREAT)
        	fd = open(rc->file, rc->mode, 0644);
    	else
        	fd = open(rc->file, rc->mode); */
		if (fd < 0)
		{
			printf("open file \"%s\" failed\n", rcmd->file);
			return(1);
		}
		pid = fork1();
		if (pid == 0)
		{
			setup_signals_child();
			if (dup2(fd, rcmd->fd) < 0)
			{
            	perror("dup2");
            	exit(1);
        	}
        	close(fd);
			if(rcmd->cmd->type == EXEC) //se funziona lascia, altrimenti rivedi
				rcmd->cmd->type = EXECP;
        	exit(ft_runcmd(rcmd->cmd, envp)); //va bene che sia un figlio nel figlio o ci puo essere caso builtin come in pipe?
		}
		close(fd);
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
    		return WEXITSTATUS(status);
		else
    		return 1;
	}
	else if (cmd->type == PIPE)
	{
		pcmd = (t_pipecmd *)cmd;
		if (pipe(p) < 0)
			ft_exit_err("pipe");
		pid_left = fork1();
		if (pid_left == 0)
		{
			setup_signals_child();
			dup2(p[1], 1);
			close(p[0]);
			close(p[1]);
			if(pcmd->left->type == EXEC)
				pcmd->left->type = EXECP;
			exit(ft_runcmd(pcmd->left, envp));
		}
		pid_right = fork1();
		if (pid_right == 0)
		{
			setup_signals_child();
			dup2(p[0], 0);
			close(p[0]);
			close(p[1]);
			if(pcmd->right->type == EXEC)
				pcmd->right->type = EXECP;
			exit(ft_runcmd(pcmd->right, envp));
		}
		close(p[0]);
		close(p[1]);
		waitpid(pid_left, NULL, 0);
		waitpid(pid_right, &status_right, 0);
		if (WIFEXITED(status_right))
    		return WEXITSTATUS(status_right);
		else
    		return 1;
	}
	else if (cmd->type == AND)
	{
        acmd = (t_andcmd *) cmd;
        status_left = ft_runcmd(acmd->left, envp);
        if (status_left == 0) // se left è andato a buon fine, eseguo right
            return ft_runcmd(acmd->right, envp);
		else  // left ha fallito: non eseguo right, torno status_left
            return status_left;
	}
	else if (cmd->type == OR)
	{
            ocmd = (t_orcmd *) cmd;
            status_left = ft_runcmd(ocmd->left, envp);
            if (status_left != 0)  // se left è fallito, eseguo right
                return ft_runcmd(ocmd->right, envp);
            else // left è andato a buon fine: salto right, torno 0
                return 0;
    }
	else if (cmd->type == HEREDOC)
	{
            printf("CASO HEREDOC da implementare in runcmd");
    }
	else
	{
		printf("runcmd error: unknown cmd type %d\n", cmd->type);
		return 1;
	}
	return 1;
}
