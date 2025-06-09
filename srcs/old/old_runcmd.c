#include "../includes/minishell.h"

int	ft_check_builtin(t_execcmd *ecmd)
{
		if (ft_strncmp(ecmd->argv[0], "pwd", 3) == 0)
			return(builtin_pwd(), 1);
		else if (strncmp(ecmd->argv[0], "echo", 4) == 0)
			return(builtin_echo(ecmd), 1);
		else if (strncmp(ecmd->argv[0], "cd", 2) == 0)
			return(builtin_cd(ecmd), 1);
		else if (strncmp(ecmd->argv[0], "exit", 4) == 0 && ecmd->argv[0][4] == '\0')
			return(builtin_exit(ecmd), 1);
		else
			return(0);
}

void ft_runcmd(t_cmd *cmd)
{
	int p[2];
	t_execcmd *ecmd;
	t_listcmd *lcmd;
	t_pipecmd *pcmd;
	t_redircmd *rcmd;

	if (cmd == NULL)
		return;
	if (cmd->type == BUILTIN)
	{
		ecmd = (t_execcmd *)cmd;
		if (ecmd->argv[0] == NULL)
			return;
		if (strncmp(ecmd->argv[0], "pwd", 3) == 0 && ecmd->argv[0][3] == '\0')
			builtin_pwd();
		else if (strncmp(ecmd->argv[0], "echo", 4) == 0 && ecmd->argv[0][4] == '\0')
			builtin_echo(ecmd);
		else if (strncmp(ecmd->argv[0], "cd", 2) == 0 && ecmd->argv[0][2] == '\0')
			builtin_cd(ecmd);
		else if (strncmp(ecmd->argv[0], "exit", 4) == 0 && ecmd->argv[0][4] == '\0')
			builtin_exit(ecmd);
		else
		{
			printf("minishell: %s: command not found\n", ecmd->argv[0]);
			return;
		}
	}
	else if (cmd->type == EXEC)
	{
		ecmd = (t_execcmd *)cmd;
		if (ecmd->argv[0] == NULL)
			return;
		if (fork1() == 0)
		{
			/* printf("ok %s\n", ecmd->argv[0]); */
			execvp(ecmd->argv[0], ecmd->argv);
			printf("exec %s failed\n", ecmd->argv[0]);
			exit(1);
		}
		wait(NULL);
		return;
	}
	else if (cmd->type == EXECP)
	{
		ecmd = (t_execcmd *)cmd;
		if (ecmd->argv[0] == NULL)
			return;
		/* if(fork1() == 0)
		{ */
		execvp(ecmd->argv[0], ecmd->argv);
		printf("exec %s failed\n", ecmd->argv[0]);
		/* exit(1); */
		/* }
		wait(NULL); */
		return;
	}
	else if (cmd->type == REDIR)
	{
		rcmd = (t_redircmd *)cmd;
		close(rcmd->fd);
		if (open(rcmd->file, rcmd->mode) < 0)
		{
			printf("open %s failed\n", rcmd->file);
			exit(1);
		}
		ft_runcmd(rcmd->cmd);
	}
	else if (cmd->type == LIST)
	{
		lcmd = (t_listcmd *)cmd;
		/* if (fork1() == 0) */
		ft_runcmd(lcmd->left);
		/* wait(NULL); */
		ft_runcmd(lcmd->right);
	}
	else if (cmd->type == PIPE)
	{
		pcmd = (t_pipecmd *)cmd;
		if (pipe(p) < 0)
			ft_exit_err("pipe");
		if (fork1() == 0)
		{
			dup2(p[1], 1);
			close(p[0]);
			close(p[1]);
			if (pcmd->left->type == EXEC)
				pcmd->left->type = EXECP;
			ft_runcmd(pcmd->left);
		}
		wait(NULL);
		if (fork1() == 0)
		{
			dup2(p[0], 0);
			close(p[0]);
			close(p[1]);
			if (pcmd->right->type == EXEC)
				pcmd->right->type = EXECP;
			ft_runcmd(pcmd->right);
		}
		close(p[0]);
		close(p[1]);
		wait(NULL);
	}
	else
	{
		if (fork1() == 0)
			ft_exit_err("runcmd error\n");
	}
	return;
}
