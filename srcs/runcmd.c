#include "../includes/minishell.h"

void update_exit_status(int status, int *p_last_exit_status)
{
    *p_last_exit_status = status;
}

int	ft_check_builtin(t_execcmd *ecmd, char ***envp, int *p_last_exit_status)
{
    int exit_status;
    
    if (ecmd->argv[0] == NULL)
        return (-1); //no builtin
    if (ft_strncmp(ecmd->argv[0], "pwd", 4) == 0)
    {
        exit_status = builtin_pwd();
        return (update_exit_status(exit_status, p_last_exit_status), exit_status);
    }
    else if (ft_strncmp(ecmd->argv[0], "echo", 5) == 0)
    {
        exit_status = builtin_echo(ecmd);
        return (update_exit_status(exit_status, p_last_exit_status), exit_status);
    }
    else if (ft_strncmp(ecmd->argv[0], "cd", 3) == 0)
    {
        exit_status = builtin_cd(ecmd, *envp);
        return (update_exit_status(exit_status, p_last_exit_status), exit_status);
    }
    else if (ft_strncmp(ecmd->argv[0], "exit", 5) == 0)
    {
        exit_status = builtin_exit(ecmd);
        return (update_exit_status(exit_status, p_last_exit_status), exit_status);
    }
    else if (ft_strncmp(ecmd->argv[0], "env", 4) == 0)
    {
        exit_status = builtin_env(*envp);
        return (update_exit_status(exit_status, p_last_exit_status), exit_status);
    }
    else if (ft_strncmp(ecmd->argv[0], "export", 7) == 0)
    {
        exit_status = builtin_export(ecmd, envp);
        return (update_exit_status(exit_status, p_last_exit_status), exit_status);
    }
    else if (ft_strncmp(ecmd->argv[0], "unset", 6) == 0)
    {
        exit_status = builtin_unset(ecmd, envp);
        return (update_exit_status(exit_status, p_last_exit_status), exit_status);
    }
    return (-1); //no builtin
}

// ora restituisce 1 se fd==1 è stato rediretto, 2 se fd==0 rediretto, 0 altrimenti
static int apply_redirs(t_cmd *cmd, int *p_last_exit_status)
{
    t_redircmd *redirs[32];
    int         n = 0, i;
    int         did_redirect_std_in_out = 0;
    t_cmd      *cur = cmd;

    while (cur && cur->type == REDIR) {
        redirs[n++] = (t_redircmd *)cur;
        cur = redirs[n-1]->cmd;
    }
    for (i = n - 1; i >= 0; i--) {
        t_redircmd *rc = redirs[i];
        if (rc->fd == STDOUT_FILENO)
            did_redirect_std_in_out = 1;
		else if (rc->fd == STDIN_FILENO)
            did_redirect_std_in_out = 2;
        int flags = (rc->fd == STDIN_FILENO)
                    ? O_RDONLY
                    : ((rc->mode & O_APPEND)
                       ? O_WRONLY|O_APPEND|O_CREAT
                       : O_WRONLY|O_CREAT|O_TRUNC);
        int fd = open(rc->file, flags, 0644);
        if (fd < 0) {
            ft_putstr_fd(rc->file, 2);
            if (errno == ENOENT)
    			ft_putstr_fd(": No such file or directory\n", 2);
			else
    			ft_putstr_fd(": Permission denied\n", 2);
            update_exit_status(1, p_last_exit_status);
            exit(1);
        }
        if (dup2(fd, rc->fd) < 0) {
            perror("dup2");
            close(fd);
            update_exit_status(1, p_last_exit_status);
            exit(1);
        }
        close(fd);
    }
    return did_redirect_std_in_out;
}

/* funzione per rimuovere argv[i]=="" empty strings */
static void remove_empty_args(t_execcmd *ecmd)
{
    int read_i;
    int write_i;

    read_i  = 0;
    write_i = 0;
    while (ecmd->argv[read_i])
    {
        if (ecmd->argv[read_i][0] != '\0')
        {
            ecmd->argv[write_i]     = ecmd->argv[read_i];
            ecmd->eargv[write_i]    = ecmd->eargv[read_i];
            ecmd->qtype[write_i]    = ecmd->qtype[read_i];
            ecmd->allocated[write_i]= ecmd->allocated[read_i];
            write_i++;
        }
        read_i++;
    }
    ecmd->argv[write_i]  = NULL;
    ecmd->eargv[write_i] = NULL;
}

int	ft_runcmd(t_cmd *cmd, char ***envp, int *p_last_exit_status)
{
	int				p[2];
	int				status_left;
	int				status_right;
	int				status;
	pid_t			pid;
	pid_t			pid_left;
	pid_t			pid_right;
	pid_t			pid_here;
	//int				fd;
	t_execcmd		*ecmd;
	t_pipecmd		*pcmd;
	//t_redircmd		*rcmd;
	t_andcmd		*acmd;
	t_orcmd			*ocmd;
	t_heredoccmd	*hcmd;

	if (cmd == NULL)
		return (0);
	if (cmd->type == EXEC)
	{
		ecmd = (t_execcmd *)cmd;
		expand_wildcards(ecmd);
		expand_variables(ecmd, *envp, p_last_exit_status);
		remove_empty_args(ecmd);
		if (ecmd->argv[0] == NULL)
			return (update_exit_status(0, p_last_exit_status), 0);
		apply_redirs(cmd, p_last_exit_status);
		status = ft_check_builtin(ecmd, envp, p_last_exit_status);
    	if (status != -1)
			return (status); //last_exit_status già aggiornato in ft_check_builtin
		pid = fork1();
		if (pid == 0)
		{
			setup_signals_child();
			execve(find_path(ecmd->argv, *envp), ecmd->argv, *envp);
			if (errno == ENOENT)
			{
				update_exit_status(127, p_last_exit_status);
            	exit(127);
			}
			else
			{
				ft_putstr_fd(" Is a directory\n", 2);
				update_exit_status(126, p_last_exit_status);
            	exit(126);
			}
		}
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			return (update_exit_status(WEXITSTATUS(status), p_last_exit_status), WEXITSTATUS(status));
		else
			return (update_exit_status(1, p_last_exit_status), 1);
	}
	else if (cmd->type == EXECP)
	{
		ecmd = (t_execcmd *)cmd;
		expand_wildcards(ecmd);
		expand_variables(ecmd, *envp, p_last_exit_status);
		remove_empty_args(ecmd);
		if (ecmd->argv[0] == NULL)
			return(update_exit_status(0, p_last_exit_status), 0);
		apply_redirs(cmd, p_last_exit_status);
		status = ft_check_builtin(ecmd, envp, p_last_exit_status);
    	if (status != -1)
			return(status); //exit status già aggiornato in ft_check_builtin
		execve(find_path(ecmd->argv, *envp), ecmd->argv, *envp);
		if (errno == ENOENT)
		{
			update_exit_status(127, p_last_exit_status);
            exit(127);
		}
		else
		{
			ft_putstr_fd(" Is a directory\n", 2);
			update_exit_status(126, p_last_exit_status);
            exit(126);
		}
	}
	else if (cmd->type == REDIR)
	{
		t_redircmd *redirs[32];
		int         n = 0, i;
		t_cmd      *cur = cmd;
		int         saved_stdin  = dup(STDIN_FILENO);
		int         saved_stdout = dup(STDOUT_FILENO);
		int         status = 0;

		if (saved_stdin < 0 || saved_stdout < 0) {
			perror("dup");
			update_exit_status(1, p_last_exit_status);
			return 1;
		}

		// 1) Raccogli tutte le REDIR in un array
		while (cur->type == REDIR) {
			redirs[n++] = (t_redircmd *)cur;
			cur = redirs[n-1]->cmd;
		}

		// 2) Applica bottom-up: prima inner (last parsed), poi outer
		for (i = n - 1; i >= 0; i--) {
			t_redircmd *rc = redirs[i];
			int flags = (rc->fd == STDIN_FILENO)
						? O_RDONLY
						: ((rc->mode & O_APPEND)
						? O_WRONLY|O_APPEND|O_CREAT
						: O_WRONLY|O_CREAT|O_TRUNC);
			int fd = open(rc->file, flags, 0644);
			if (fd < 0) {
				ft_putstr_fd(rc->file, 2);
				if (errno == ENOENT)
					ft_putstr_fd(": No such file or directory\n", 2);
				else
					ft_putstr_fd(": Permission denied\n", 2);
				update_exit_status(1, p_last_exit_status);
				status = 1;
				goto restore_fds;
			}
			if (dup2(fd, rc->fd) < 0) {
				perror("dup2");
				close(fd);
				update_exit_status(1, p_last_exit_status);
				status = 1;
				goto restore_fds;
			}
			close(fd);
		}

		// 3) Esegui finalmente il comando vero (EXEC, PIPE, etc.)
		status = ft_runcmd(cur, envp, p_last_exit_status);

		restore_fds:
		// 4) Ripristina sempre stdin/stdout originali
		dup2(saved_stdin,  STDIN_FILENO);  close(saved_stdin);
		dup2(saved_stdout, STDOUT_FILENO); close(saved_stdout);
		return status;
	}
	else if (cmd->type == PIPE)
	{
		pcmd = (t_pipecmd *)cmd;
		if (pipe(p) < 0)
			ft_exit_err_n("pipe", p_last_exit_status);
		pid_left = fork1();
		if (pid_left == 0)
		{
			setup_signals_child();
			int redir_stdout = apply_redirs(pcmd->left, p_last_exit_status);
        	if (redir_stdout != 1)
				dup2(p[1], 1);
			close(p[0]);
			close(p[1]);
			if (pcmd->left->type == EXEC)
				pcmd->left->type = EXECP;
			exit(ft_runcmd(pcmd->left, envp, p_last_exit_status));
		}
		pid_right = fork1();
		if (pid_right == 0)
		{
			setup_signals_child();
			int redir_stdin = apply_redirs(pcmd->right, p_last_exit_status); 
        	if (redir_stdin != 2)
				dup2(p[0], 0);
			close(p[0]);
			close(p[1]);
			if (pcmd->right->type == EXEC)
				pcmd->right->type = EXECP;
			exit(ft_runcmd(pcmd->right, envp, p_last_exit_status));
		}
		close(p[0]);
		close(p[1]);
		waitpid(pid_left, NULL, 0);
		waitpid(pid_right, &status_right, 0);
		if (WIFEXITED(status_right))
			return (update_exit_status(WEXITSTATUS(status_right), p_last_exit_status), WEXITSTATUS(status_right));
		else
			return (update_exit_status(1, p_last_exit_status), 1);
	}
	else if (cmd->type == AND)
	{
		acmd = (t_andcmd *)cmd;
		status_left = ft_runcmd(acmd->left, envp, p_last_exit_status);
		if (status_left == 0)
		{
			status_right = ft_runcmd(acmd->right, envp, p_last_exit_status);
			return (update_exit_status(status_right, p_last_exit_status), status_right);
		}
		else
			return (update_exit_status(status_left, p_last_exit_status), status_left);
	}
	else if (cmd->type == OR)
	{
		ocmd = (t_orcmd *)cmd;
		status_left = ft_runcmd(ocmd->left, envp, p_last_exit_status);
		if (status_left != 0)
		{
			status_right = ft_runcmd(ocmd->right, envp, p_last_exit_status);
			return (update_exit_status(status_right, p_last_exit_status), status_right);
		}
		else
			return (update_exit_status(0, p_last_exit_status), 0);
	}
	else if (cmd->type == HEREDOC)
	{
		hcmd = (t_heredoccmd *)cmd;
		if (pipe(p) < 0)
			ft_exit_err("pipe");
		pid_here = fork1();
		if (pid_here == 0)
		{
			close(p[0]);
			ft_child(p, hcmd, *envp);
			exit(0);
		}
		close(p[1]);
		waitpid(pid_here, &status, 0);
		pid = fork1();
		if (pid == 0)
		{
			dup2(p[0], STDIN_FILENO);
			close(p[0]);
			exit(ft_runcmd(hcmd->cmd, envp, p_last_exit_status));
		}
		close(p[0]);
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			return (update_exit_status(WEXITSTATUS(status), p_last_exit_status), WEXITSTATUS(status));
		else
			return (update_exit_status(1, p_last_exit_status), 1);
	}
	else
	{
		printf("runcmd error: unknown cmd type %d\n", cmd->type);
		return (1);
	}
	return (1);
}

void	ft_child(int *fd, t_heredoccmd *hcmd, char **envp)
{
	char	*line;

	close(fd[0]);
	line = get_next_line(STDIN_FILENO);
	while (line != NULL)
	{
		if (ft_strncmp(line, hcmd->lim_start, ft_strlen(hcmd->lim_start)) == 0)
		{
			free(line);
			close(fd[1]);
			return;
		}
		if (hcmd->expand && ft_strchr(line, '$'))
			perform_expansion(&line, envp);
		write(fd[1], line, ft_strlen(line));
		free(line);
		line = get_next_line(STDIN_FILENO);
	}
	close(fd[1]);
}
