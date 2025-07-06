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
	int				fd;
	t_execcmd		*ecmd;
	t_pipecmd		*pcmd;
	t_redircmd		*rcmd;
	t_andcmd		*acmd;
	t_orcmd			*ocmd;
	t_heredoccmd	*hcmd;

	if (cmd == NULL)
		return (0);
	if (cmd->type == EXEC)
	{
		ecmd = (t_execcmd *)cmd;
		expand_variables(ecmd, *envp);
		if (ecmd->argv[0] == NULL)
			return (update_exit_status(0, p_last_exit_status), 0);
		status = ft_check_builtin(ecmd, envp, p_last_exit_status);
    	if (status != -1)
			return (status); //last_exit_status già aggiornato in ft_check_builtin
		pid = fork1();
		if (pid == 0)
		{
			setup_signals_child();
			execve(find_path(ecmd->argv, *envp), ecmd->argv, *envp);
			printf("exec %s failed\n", ecmd->argv[0]);
			exit(127);
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
		if (ecmd->argv[0] == NULL)
			return(update_exit_status(0, p_last_exit_status), 0);
		status = ft_check_builtin(ecmd, envp, p_last_exit_status);
    	if (status != -1)
			return(status); //exit status già aggiornato in ft_check_builtin
		execve(find_path(ecmd->argv, *envp), ecmd->argv, *envp);
		printf("exec %s failed\n", ecmd->argv[0]);
		exit(127);
	}
	else if (cmd->type == REDIR)
	{
		rcmd = (t_redircmd *)cmd;
		fd = open(rcmd->file, rcmd->mode, 0644);
		if (fd < 0)
		{
			printf("open file \"%s\" failed\n", rcmd->file);
			return (update_exit_status(1, p_last_exit_status), 1);
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
			if (rcmd->cmd->type == EXEC)
				rcmd->cmd->type = EXECP;
			exit(ft_runcmd(rcmd->cmd, envp, p_last_exit_status));
		}
		close(fd);
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			return(update_exit_status(WEXITSTATUS(status), p_last_exit_status), WEXITSTATUS(status));
		else
			return (update_exit_status(1, p_last_exit_status), 1);
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
			ft_child(p, hcmd->lim_start);
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

void	ft_child(int *fd, char *limiter)
{
	char	*line;

	close(fd[0]);
	line = get_next_line(STDIN_FILENO);
	while (line != NULL)
	{
		if (ft_strncmp(line, limiter, ft_strlen(limiter)) == 0)
		{
			free(line);
			close(fd[1]);
			exit(0);
		}
		write(fd[1], line, ft_strlen(line));
		free(line);
		line = get_next_line(STDIN_FILENO);
	}
	close(fd[1]);
}
