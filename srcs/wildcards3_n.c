/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards3_n.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierant & sfelici <marvin@student.42ro    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 17:34:59 by mpierant &        #+#    #+#             */
/*   Updated: 2025/07/17 18:46:41 by mpierant &       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	record_match(t_execcmd *ecmd, int *t_count, char *name)
{
	char	*full_path;

	full_path = malloc(ft_strlen(name) + 3);
	ft_strlcpy(full_path, "./", ft_strlen(name + 3));
	ft_strlcat(full_path, name, ft_strlen(name + 3));
	ecmd->argv[*t_count] = full_path;
	ecmd->qtype[*t_count] = 'a';
	ecmd->allocated[*t_count] = 1;
	(*t_count)++;
}

static int	iterate_dir(DIR *d, char *match_pat, t_execcmd *ecmd, int *t_count)
{
	struct dirent	*e;
	int				found;

	found = 0;
	e = readdir(d);
	while (e && *t_count < MAX_ARGS - 1)
	{
		if (!(e->d_name[0] == '.' && match_pat[0] != '.'))
		{
			if (match_star(match_pat, e->d_name))
			{
				record_match(ecmd, t_count, e->d_name);
				found = 1;
			}
		}
		e = readdir(d);
	}
	return (found);
}

// supporto per pattern che iniziano con "./"
int	handle_dot_slash_pattern(char *pat, t_execcmd *ecmd, int *t_count)
{
	DIR		*d;
	char	*match_pat;
	int		found;

	found = 0;
	if (strncmp(pat, "./", 2) == 0)
		match_pat = pat + 2;
	else
		match_pat = pat;
	d = opendir(".");
	if (d)
	{
		found = iterate_dir(d, match_pat, ecmd, t_count);
		closedir(d);
	}
	return (found);
}
