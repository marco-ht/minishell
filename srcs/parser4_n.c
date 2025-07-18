/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser4.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierant & sfelici <marvin@student.42ro    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 19:38:51 by mpierant &        #+#    #+#             */
/*   Updated: 2025/07/18 19:39:56 by mpierant &       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	handle_exec_cmd(t_cmd *cmd)
{
	int			i;
	t_execcmd	*ecmd;

	ecmd = (t_execcmd *)cmd;
	i = -1;
	while (ecmd->argv[++i])
		*ecmd->eargv[i] = 0;
}

void	handle_redir_cmd(t_cmd *cmd)
{
	t_redircmd	*rcmd;

	rcmd = (t_redircmd *)cmd;
	nulterminate(rcmd->cmd);
	*rcmd->efile = 0;
}

void	handle_pipe_cmd(t_cmd *cmd)
{
	t_pipecmd	*pcmd;

	pcmd = (t_pipecmd *)cmd;
	nulterminate(pcmd->left);
	nulterminate(pcmd->right);
}

void	handle_and_cmd(t_cmd *cmd)
{
	t_andcmd	*acmd;

	acmd = (t_andcmd *)cmd;
	nulterminate(acmd->left);
	nulterminate(acmd->right);
}

void	handle_or_cmd(t_cmd *cmd)
{
	t_orcmd	*ocmd;

	ocmd = (t_orcmd *)cmd;
	nulterminate(ocmd->left);
	nulterminate(ocmd->right);
}
