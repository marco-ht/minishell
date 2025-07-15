#include "../includes/minishell.h"

void	free_exec(t_execcmd *ex)
{
	int	i;

	i = -1;
	while(ex->argv[++i])
	{
		if(ex->allocated[i])
			free(ex->argv[i]);
	}
	free(ex);
}

void	free_tree(t_cmd *cmd)
{
	t_execcmd	*execmd;
	t_redircmd	*redircmd;
	t_pipecmd	*pipecmd;
	t_andcmd 	*acmd;
	t_orcmd 	*ocmd;
	t_heredoccmd *hcmd;

	if (cmd == NULL)
		return ;
	if(cmd->type == EXEC)
	{
		execmd = (t_execcmd *)cmd;
		free_exec(execmd);
	}
	else if (cmd->type == PIPE)
	{
		pipecmd = (t_pipecmd *)cmd;
		free_tree(pipecmd->left);
		free_tree(pipecmd->right);
		free(pipecmd);
	}
	else if(cmd->type == REDIR)
	{
		redircmd = (t_redircmd *)cmd;
		free_tree(redircmd->cmd);
		if (redircmd->allocated)
			free(redircmd->file);
		free(redircmd);
	}
	else if (cmd->type == AND)
	{
		acmd = (t_andcmd *)cmd;
		free_tree(acmd->left);
		free_tree(acmd->right);
		free(acmd);
	}
	else if (cmd->type == OR)
	{
		ocmd = (t_orcmd *)cmd;
		free_tree(ocmd->left);
		free_tree(ocmd->right);
		free(ocmd);
	}
	else if (cmd->type == HEREDOC)
	{
		hcmd = (t_heredoccmd *)cmd;
		free_tree(hcmd->cmd);
		free(hcmd);
	}
}
