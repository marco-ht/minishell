#include "../includes/minishell.h"

void ft_runcmd(t_cmd *cmd)
{
	int p[2];
	t_execcmd *ecmd;
	t_listcmd *lcmd;
	t_pipecmd *pcmd;
	t_redircmd *rcmd;


	if (cmd == NULL)
		return ;
	if(cmd->type == BUILTIN)
	{
		ecmd = (t_execcmd *)cmd;
		if (ecmd->argv[0] == NULL)
			return ;
		if (strcmp(ecmd->argv[0], "pwd") == 0)
			builtin_pwd(ecmd);
		/*else if (strcmp(ecmd->argv[0], "cd") == 0)
			builtin_cd(ecmd);
		else if (strcmp(ecmd->argv[0], "echo") == 0)
			builtin_echo(ecmd);*/
		else {
			printf("Builtin %s not found", ecmd->argv[0]);
			exit(1);
		}
	}
	else if (cmd->type == EXEC)
	{
		ecmd = (t_execcmd *)cmd;
		if (ecmd->argv[0] == NULL)
			return ;
		/* if(fork1() == 0)
		{ */
		/* printf("ok %s\n", ecmd->argv[0]); */
		execvp(ecmd->argv[0], ecmd->argv);
		printf("exec %s failed\n", ecmd->argv[0]);
		/* } */
		exit(1);
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
		if (fork1() == 0)
			ft_runcmd(lcmd->left);
		wait(NULL);
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
			ft_runcmd(pcmd->left);
		}
		wait(NULL);
		if (fork1() == 0)
		{
			dup2(p[0], 0);
			close(p[0]);
			close(p[1]);
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
	exit(0);
}
