/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierant & sfelici <marvin@student.42ro    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 19:24:45 by mpierant &        #+#    #+#             */
/*   Updated: 2025/07/18 19:40:55 by mpierant &       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	handle_heredoc_cmd(t_cmd *cmd)
{
	t_heredoccmd	*hd;

	hd = (t_heredoccmd *)cmd;
	*hd->lim_end = '\0';
	nulterminate(hd->cmd);
}

// NUL-terminate all the tree strings.
t_cmd	*nulterminate(t_cmd *cmd)
{
	if (cmd == 0)
		return (0);
	if (cmd->type == EXEC)
		handle_exec_cmd(cmd);
	else if (cmd->type == REDIR)
		handle_redir_cmd(cmd);
	else if (cmd->type == PIPE)
		handle_pipe_cmd(cmd);
	else if (cmd->type == AND)
		handle_and_cmd(cmd);
	else if (cmd->type == OR)
		handle_or_cmd(cmd);
	else if (cmd->type == HEREDOC)
		handle_heredoc_cmd(cmd);
	return (cmd);
}
