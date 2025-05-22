#include "../includes/minishell.h"

void	free_exec(t_execcmd *ex)
{
	free(ex);
}

void	free_tree(t_cmd *cmd)
{
	t_execcmd	*execmd;
	t_redircmd	*redircmd;
	t_listcmd	*listcmd;
	t_pipecmd	*pipecmd;

	if (cmd == NULL)
		return ;
	if(cmd->type == EXEC || cmd->type == BUILTIN)
	{
		execmd = (t_execcmd *)cmd;
		free_exec(execmd);
	}
	else if(cmd->type == REDIR)
	{
		redircmd = (t_redircmd *)cmd;
		free_tree(redircmd->cmd);
		free(redircmd);

	}
	else if (cmd->type == LIST)
	{
		listcmd = (t_listcmd *)cmd;
		free_tree(listcmd->left);
		free_tree(listcmd->right);
		free(listcmd);
	}
	else if (cmd->type == PIPE)
	{
		pipecmd = (t_pipecmd *)cmd;
		free_tree(pipecmd->left);
		free_tree(pipecmd->right);
		free(pipecmd);
	}

}
