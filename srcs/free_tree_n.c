/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_tree_n.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierant & sfelici <marvin@student.42ro    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 14:08:11 by mpierant &        #+#    #+#             */
/*   Updated: 2025/07/22 17:35:57 by mpierant &       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_redir_cmd(t_redircmd *redircmd)
{
	free_tree(redircmd->cmd);
	if (redircmd->allocated)
		free(redircmd->file);
	free(redircmd);
}

void	free_and_cmd(t_andcmd *acmd)
{
	free_tree(acmd->left);
	free_tree(acmd->right);
	free(acmd);
}

void	free_or_cmd(t_orcmd *ocmd)
{
	free_tree(ocmd->left);
	free_tree(ocmd->right);
	free(ocmd);
}

void	free_heredoc_cmd(t_heredoccmd *hcmd)
{
	free_tree(hcmd->cmd);
	free(hcmd);
}

void	free_tree(t_cmd *cmd)
{
	if (cmd == NULL)
		return ;
	if (cmd->type == EXEC)
		free_exec((t_execcmd *)cmd);
	else if (cmd->type == EXECP)
    	free_exec((t_execcmd *)cmd);
	else if (cmd->type == PIPE)
		free_pipe_cmd((t_pipecmd *)cmd);
	else if (cmd->type == REDIR)
		free_redir_cmd((t_redircmd *)cmd);
	else if (cmd->type == AND)
		free_and_cmd((t_andcmd *)cmd);
	else if (cmd->type == OR)
		free_or_cmd((t_orcmd *)cmd);
	else if (cmd->type == HEREDOC)
		free_heredoc_cmd((t_heredoccmd *)cmd);
}
