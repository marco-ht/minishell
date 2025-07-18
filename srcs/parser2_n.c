/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierant & sfelici <marvin@student.42ro    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 19:17:08 by mpierant &        #+#    #+#             */
/*   Updated: 2025/07/18 19:20:36 by mpierant &       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// skip whitespaces and return bolean,
//= 1 if the character exists and it is one of the toks
int	peek(char **ps, char *es, char *toks)
{
	char	*s;
	char	*whitespace;

	whitespace = " \t\r\n\v";
	s = *ps;
	while (s < es && ft_strchr(whitespace, *s))
		s++;
	*ps = s;
	return (*s && ft_strchr(toks, *s));
}

t_cmd	*ft_parsecmd(char *s, int *status)
{
	char	*es;
	char	*p;
	t_cmd	*cmd;

	es = s + strlen(s);
	p = es - 1;
	while (p >= s && (*p == ' ' || *p == '\t' || *p == '\n'))
		p--;
	if (p >= s && *p == '|')
	{
		*status = 1;
		return (NULL);
	}
	*status = 0;
	cmd = parseandor(&s, es);
	peek(&s, es, "");
	if (s != es)
	{
		printf("minishell: syntax error near '%s'\n", s);
		return (free_tree(cmd), NULL);
	}
	nulterminate(cmd);
	return (cmd);
}

t_cmd	*parsepipe(char **ps, char *es)
{
	t_cmd	*cmd;
	char	*backup;
	int		tok;

	cmd = parseexec(ps, es);
	if (!cmd)
		return (NULL);
	while (1)
	{
		backup = *ps;
		tok = gettoken(ps, es, 0, 0);
		if (tok == '|')
			cmd = ft_pipecmd(cmd, parsepipe(ps, es));
		else
		{
			*ps = backup;
			break ;
		}
	}
	return (cmd);
}

t_cmd	*parseblock(char **ps, char *es)
{
	t_cmd	*cmd;

	if (!peek(ps, es, "("))
	{
		printf("parseblock '(' missing)");
		return (NULL);
	}
	gettoken(ps, es, 0, 0);
	cmd = parseandor(ps, es);
	if (!peek(ps, es, ")"))
	{
		printf("parseblock ')' missing)");
		return (free(cmd), NULL);
	}
	gettoken(ps, es, 0, 0);
	cmd = parseredirs(cmd, ps, es);
	return (cmd);
}
