/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards1d.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierant & sfelici <marvin@student.42ro    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 23:34:32 by mpierant &        #+#    #+#             */
/*   Updated: 2025/07/17 23:40:27 by mpierant &       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	process_file_matches(t_expand_vars *vars)
{
	vars->e2 = readdir(vars->d2);
	while (vars->e2)
	{
		if (vars->e2->d_name[0] == '.')
		{
			vars->e2 = readdir(vars->d2);
			continue ;
		}
		if (match_star(vars->file_pat, vars->e2->d_name))
		{
			vars->tmp = ft_strjoin(vars->e1->d_name, "/");
			vars->tmp_path = ft_strjoin(vars->tmp, vars->e2->d_name);
			free(vars->tmp);
			vars->matches[vars->match_count] = vars->tmp_path;
			vars->match_count++;
			vars->found = 1;
		}
		vars->e2 = readdir(vars->d2);
	}
}

static void	process_subdir(t_expand_vars *vars)
{
	vars->d2 = opendir(vars->e1->d_name);
	if (vars->d2)
	{
		process_file_matches(vars);
		closedir(vars->d2);
	}
}

static void	process_entry(t_expand_vars *vars)
{
	if (vars->e1->d_name[0] == '.')
		return ;
	if (match_star(vars->dir_pat, vars->e1->d_name))
	{
		if (stat(vars->e1->d_name, &vars->st) == 0 && S_ISDIR(vars->st.st_mode))
		{
			process_subdir(vars);
		}
	}
}

void	process_directory_matches(t_expand_vars *vars)
{
	vars->d1 = opendir(".");
	if (vars->d1)
	{
		vars->e1 = readdir(vars->d1);
		while (vars->e1)
		{
			process_entry(vars);
			vars->e1 = readdir(vars->d1);
		}
		closedir(vars->d1);
	}
}
