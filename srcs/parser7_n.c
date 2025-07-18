/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser7.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierant & sfelici <marvin@student.42ro    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 00:41:01 by mpierant &        #+#    #+#             */
/*   Updated: 2025/07/19 01:05:05 by mpierant &       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	handle_glob_expansion(t_redin *rin, int tok_type)
{
	rin->allocated = 0;
	if (tok_type == 'a' && ft_strchr(rin->file, '*'))
	{
		*rin->efile = '\0';
		rin->file = expand_redirect_glob(rin->file, &rin->allocated);
		rin->efile = rin->file + ft_strlen(rin->file);
	}
}

static t_cmd	*apply_redir(t_redin *rin, int tok, int tok_type)
{
	if (tok == '<')
	{
		rin->mode = O_RDONLY;
		rin->fd = 0;
		return (ft_redircmd(rin));
	}
	else if (tok == '>')
	{
		rin->mode = O_WRONLY | O_CREAT | O_TRUNC;
		rin->fd = 1;
		return (ft_redircmd(rin));
	}
	else if (tok == '+')
	{
		rin->mode = O_WRONLY | O_CREAT | O_APPEND;
		rin->fd = 1;
		return (ft_redircmd(rin));
	}
	else if (tok == 'h')
		return (ft_heredoccmd(rin->subcmd, tok_type, rin->file, rin->efile));
	return (rin->subcmd);
}

t_cmd	*parseredirs(t_cmd *cmd, char **ps, char *es)
{
	int		tok;
	int		tok_type;
	t_redin	rin;

	while (peek(ps, es, "<>"))
	{
		rin.subcmd = cmd;
		tok = gettoken(ps, es, 0, 0);
		tok_type = gettoken(ps, es, &rin.file, &rin.efile);
		if (tok_type != 'a' && tok_type != 's' && tok_type != 'd')
		{
			ft_putstr_fd(" missing file for redirection", 2);
			return (free_tree(cmd), NULL);
		}
		handle_glob_expansion(&rin, tok_type);
		cmd = apply_redir(&rin, tok, tok_type);
	}
	return (cmd);
}
