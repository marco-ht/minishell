/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser5.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierant & sfelici <marvin@student.42ro    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 23:09:50 by mpierant &        #+#    #+#             */
/*   Updated: 2025/07/18 23:28:26 by mpierant &       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static t_cmd	*parse_and_or_loop(t_cmd *cmd, char **ps, char *es)
{
	char	*backup;
	int		tok;

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

// Does the parsing job
t_cmd	*parseandor(char **ps, char *es)
{
	t_cmd	*cmd;

	cmd = parsepipe(ps, es);
	if (!cmd)
		return (NULL);
	cmd = parse_and_or_loop(cmd, ps, es);
	return (cmd);
}
