/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierant & sfelici <marvin@student.42ro    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 19:10:36 by mpierant &        #+#    #+#             */
/*   Updated: 2025/07/18 19:45:08 by mpierant &       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// Does the parsing job
t_cmd	*parseandor(char **ps, char *es)
{
	t_cmd	*cmd;
	char	*backup;
	int		tok;

	cmd = parsepipe(ps, es);
	if (!cmd)
		return (NULL);
	while (1)
	{
		backup = *ps;
		tok = gettoken(ps, es, NULL, NULL);
		if (tok == '&' || tok == ';')
		{
			printf("minishell: unexpected ';' or single '&' near '%s'\n", *ps);
			return (free_tree(cmd), NULL);
		}
		else if (tok == 'e')
			cmd = ft_andcmd(cmd, parsepipe(ps, es));
		else if (tok == 'o')
			cmd = ft_orcmd(cmd, parsepipe(ps, es));
		else
		{
			*ps = backup;
			break ;
		}
	}
	return (cmd);
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

t_cmd	*parseexec(char **ps, char *es)
{
	t_execcmd	*cmd;
	t_cmd		*ret;
	char		*q;
	char		*eq;
	char		*backup;
	int			tok;
	int			argc;

	if (peek(ps, es, "("))
		return (parseblock(ps, es));
	ret = ft_execcmd();
	cmd = (t_execcmd *)ret;
	argc = 0;
	ret = parseredirs(ret, ps, es);
	while (1)
	{
		backup = *ps;
		tok = gettoken(ps, es, &q, &eq);
		if (tok != 'a' && tok != 's' && tok != 'd')
		{
			*ps = backup;
			break ;
		}
		cmd->argv[argc] = q;
		cmd->eargv[argc] = eq;
		cmd->qtype[argc] = tok;
		if (++argc >= MAX_ARGS)
		{
			printf("too many args");
			return (free(cmd), free(ret), NULL);
		}
		ret = parseredirs(ret, ps, es);
	}
	cmd->argv[argc] = NULL;
	cmd->eargv[argc] = NULL;
	return (ret);
}
