#include "../includes/minishell.h"

int	ft_check_builtin(t_execcmd *ecmd, char ***envp)
{
	if (ecmd->argv[0] == NULL)
		return (0);
	if (ft_strncmp(ecmd->argv[0], "pwd", 4) == 0)
		return (builtin_pwd(), 1);
	else if (ft_strncmp(ecmd->argv[0], "echo", 5) == 0)
		return (builtin_echo(ecmd), 1);
	else if (ft_strncmp(ecmd->argv[0], "cd", 3) == 0)
		return (builtin_cd(ecmd), 1);
	else if (ft_strncmp(ecmd->argv[0], "exit", 5) == 0)
		return (builtin_exit(ecmd), 1);
	else if (ft_strncmp(ecmd->argv[0], "env", 4) == 0)
		return (builtin_env(*envp), 1);
	else if (ft_strncmp(ecmd->argv[0], "export", 7) == 0)
		return (builtin_export(ecmd, envp), 1);
	else if (ft_strncmp(ecmd->argv[0], "unset", 6) == 0)
		return (builtin_unset(ecmd, envp), 1);
	return (0);
}

int	ft_runcmd(t_cmd *cmd, char ***envp)
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
			return (0);
		if (ft_check_builtin(ecmd, envp))
			return (0);
		pid = fork1();
		if (pid == 0)
		{
			setup_signals_child();
			execve(find_path(ecmd->argv), ecmd->argv, *envp);
			printf("exec %s failed\n", ecmd->argv[0]);
			exit(127);
		}
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
		else
			return (1);
	}
	else if (cmd->type == EXECP)
	{
		ecmd = (t_execcmd *)cmd;
		if (ecmd->argv[0] == NULL)
			return (0);
		if (ft_check_builtin(ecmd, envp))
			return (0);
		execve(find_path(ecmd->argv), ecmd->argv, *envp);
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
			return (1);
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
			exit(ft_runcmd(rcmd->cmd, envp));
		}
		close(fd);
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
		else
			return (1);
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
			if (pcmd->left->type == EXEC)
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
			if (pcmd->right->type == EXEC)
				pcmd->right->type = EXECP;
			exit(ft_runcmd(pcmd->right, envp));
		}
		close(p[0]);
		close(p[1]);
		waitpid(pid_left, NULL, 0);
		waitpid(pid_right, &status_right, 0);
		if (WIFEXITED(status_right))
			return (WEXITSTATUS(status_right));
		else
			return (1);
	}
	else if (cmd->type == AND)
	{
		acmd = (t_andcmd *)cmd;
		status_left = ft_runcmd(acmd->left, envp);
		if (status_left == 0)
			return (ft_runcmd(acmd->right, envp));
		else
			return (status_left);
	}
	else if (cmd->type == OR)
	{
		ocmd = (t_orcmd *)cmd;
		status_left = ft_runcmd(ocmd->left, envp);
		if (status_left != 0)
			return (ft_runcmd(ocmd->right, envp));
		else
			return (0);
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
		pid = fork1();
		if (pid == 0)
		{
			dup2(p[0], STDIN_FILENO);
			close(p[0]);
			exit(ft_runcmd(hcmd->cmd, envp));
		}
		close(p[0]);
		waitpid(pid_here, NULL, 0);
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
		else
			return (1);
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
