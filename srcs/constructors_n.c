/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   constructors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierant & sfelici <marvin@student.42ro    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 19:53:45 by mpierant &        #+#    #+#             */
/*   Updated: 2025/07/17 15:44:39 by mpierant &       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_cmd	*ft_execcmd(void)
{
	t_execcmd	*cmd;

	cmd = malloc(sizeof(*cmd));
	if (!cmd)
		ft_exit_err("malloc");
	ft_memset(cmd, 0, sizeof(*cmd));
	cmd->type = EXEC;
	return ((t_cmd *)cmd);
}

t_cmd	*ft_pipecmd(t_cmd *left, t_cmd *right)
{
	t_pipecmd	*cmd;

	cmd = malloc(sizeof(*cmd));
	if (!cmd)
		ft_exit_err("malloc");
	ft_memset(cmd, 0, sizeof(*cmd));
	cmd->type = PIPE;
	cmd->left = left;
	cmd->right = right;
	return ((t_cmd *)cmd);
}

t_cmd	*ft_redircmd(t_redin *rin)
{
	t_redircmd	*cmd;

	cmd = malloc(sizeof(*cmd));
	if (!cmd)
		ft_exit_err("malloc");
	ft_memset(cmd, 0, sizeof(*cmd));
	cmd->type = REDIR;
	cmd->cmd = rin->subcmd;
	cmd->file = rin->file;
	cmd->efile = rin->efile;
	cmd->mode = rin->mode;
	cmd->fd = rin->fd;
	cmd->allocated = rin->allocated;
	return ((t_cmd *)cmd);
}

t_cmd	*ft_andcmd(t_cmd *left, t_cmd *right)
{
	t_andcmd	*cmd;

	cmd = malloc(sizeof(*cmd));
	if (!cmd)
		ft_exit_err("malloc");
	ft_memset(cmd, 0, sizeof(*cmd));
	cmd->type = AND;
	cmd->left = left;
	cmd->right = right;
	return ((t_cmd *)cmd);
}

t_cmd	*ft_orcmd(t_cmd *left, t_cmd *right)
{
	t_orcmd	*cmd;

	cmd = malloc(sizeof(*cmd));
	if (!cmd)
		ft_exit_err("malloc");
	ft_memset(cmd, 0, sizeof(*cmd));
	cmd->type = OR;
	cmd->left = left;
	cmd->right = right;
	return ((t_cmd *)cmd);
}
