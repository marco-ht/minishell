/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards4.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierant & sfelici <marvin@student.42ro    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 18:15:40 by mpierant &        #+#    #+#             */
/*   Updated: 2025/07/17 18:24:04 by mpierant &       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	record_hidden(t_execcmd *ecmd, int *t_count, char *name)
{
	char	*dup;

	dup = ft_strdup(name);
	ecmd->argv[*t_count] = dup;
	ecmd->qtype[*t_count] = 'a';
	ecmd->allocated[*t_count] = 1;
	(*t_count)++;
}

static int	iterate_hidden(DIR *d, char *pat, t_execcmd *ecmd, int *t_count)
{
	struct dirent	*e;
	int				found;

	found = 0;
	e = readdir(d);
	while (e && *t_count < MAX_ARGS - 1)
	{
		if (e->d_name[0] == '.' && strcmp(e->d_name, ".") != 0
			&& strcmp(e->d_name, "..") != 0)
		{
			if (match_star(pat, e->d_name))
			{
				record_hidden(ecmd, t_count, e->d_name);
				found = 1;
			}
		}
		e = readdir(d);
	}
	return (found);
}

// supporto per file nascosti
int	handle_hidden_files(char *pat, t_execcmd *ecmd, int *t_count)
{
	DIR	*d;
	int	found;

	found = 0;
	if (!(pat[0] == '.' && pat[1] == '*'))
		return (0);
	d = opendir(".");
	if (d)
	{
		found = iterate_hidden(d, pat, ecmd, t_count);
		closedir(d);
	}
	return (found);
}
