/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierant & sfelici <marvin@student.42ro    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 19:10:36 by mpierant &        #+#    #+#             */
/*   Updated: 2025/07/18 23:35:43 by mpierant &       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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
		rin.allocated = 0;
		if (tok_type == 'a' && ft_strchr(rin.file, '*'))
		{
			*rin.efile = '\0';
			rin.file = expand_redirect_glob(rin.file, &rin.allocated);
			rin.efile = rin.file + ft_strlen(rin.file);
		}
		if (tok == '<')
		{
			rin.mode = O_RDONLY;
			rin.fd = 0;
			cmd = ft_redircmd(&rin);
		}
		else if (tok == '>')
		{
			rin.mode = O_WRONLY | O_CREAT | O_TRUNC;
			rin.fd = 1;
			cmd = ft_redircmd(&rin);
		}
		else if (tok == '+')
		{
			rin.mode = O_WRONLY | O_CREAT | O_APPEND;
			rin.fd = 1;
			cmd = ft_redircmd(&rin);
		}
		else if (tok == 'h')
			cmd = ft_heredoccmd(cmd, tok_type, rin.file, rin.efile);
	}
	return (cmd);
}
