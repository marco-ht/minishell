/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards1c.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierant & sfelici <marvin@student.42ro    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 22:56:50 by mpierant &        #+#    #+#             */
/*   Updated: 2025/07/17 22:58:00 by mpierant &       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	handle_slash_wildcard(t_execcmd *ecmd, t_expand_vars *vars)
{
	vars->slash = ft_strchr(vars->pat, '/');
	if (vars->slash)
	{
		vars->found = 0;
		vars->match_count = 0;
		vars->dir_len = vars->slash - vars->pat;
		vars->dir_pat = ft_strndup(vars->pat, vars->dir_len);
		vars->file_pat = ft_strdup(vars->slash + 1);
		process_directory_matches(vars);
		add_directory_matches_to_argv(ecmd, vars);
		free(vars->dir_pat);
		free(vars->file_pat);
		if (vars->found == 0)
		{
			ecmd->argv[vars->t_count] = vars->old_argv[vars->i];
			ecmd->qtype[vars->t_count] = vars->old_q[vars->i];
			ecmd->allocated[vars->t_count] = vars->old_alloc[vars->i];
			vars->t_count++;
		}
		vars->i++;
		return (1);
	}
	return (0);
}

void	process_simple_wildcard_matches(t_expand_vars *vars)
{
	vars->d1 = opendir(".");
	if (vars->d1)
	{
		vars->e1 = readdir(vars->d1);
		while (vars->e1)
		{
			if (vars->e1->d_name[0] == '.')
			{
				vars->e1 = readdir(vars->d1);
				continue ;
			}
			if (match_star(vars->pat, vars->e1->d_name))
			{
				vars->matches[vars->match_count] = ft_strdup(vars->e1->d_name);
				vars->match_count++;
				vars->found = 1;
			}
			vars->e1 = readdir(vars->d1);
		}
		closedir(vars->d1);
	}
}

void	add_simple_matches_to_argv(t_execcmd *ecmd, t_expand_vars *vars)
{
	if (vars->match_count > 0)
	{
		sort_matches(vars->matches, vars->match_count);
		vars->j = 0;
		while (vars->j < vars->match_count && vars->t_count < MAX_ARGS - 1)
		{
			ecmd->argv[vars->t_count] = vars->matches[vars->j];
			ecmd->qtype[vars->t_count] = 'a';
			ecmd->allocated[vars->t_count] = 1;
			vars->t_count++;
			++vars->j;
		}
	}
}

int	handle_simple_wildcard(t_execcmd *ecmd, t_expand_vars *vars)
{
	if (ft_strchr(vars->pat, '*'))
	{
		vars->found = 0;
		vars->match_count = 0;
		process_simple_wildcard_matches(vars);
		add_simple_matches_to_argv(ecmd, vars);
		if (vars->found == 0)
		{
			ecmd->argv[vars->t_count] = vars->old_argv[vars->i];
			ecmd->qtype[vars->t_count] = vars->old_q[vars->i];
			ecmd->allocated[vars->t_count] = vars->old_alloc[vars->i];
			vars->t_count++;
		}
		vars->i++;
		return (1);
	}
	return (0);
}

void	handle_regular_arg(t_execcmd *ecmd, t_expand_vars *vars)
{
	ecmd->argv[vars->t_count] = vars->old_argv[vars->i];
	ecmd->qtype[vars->t_count] = vars->old_q[vars->i];
	ecmd->allocated[vars->t_count] = vars->old_alloc[vars->i];
	vars->t_count++;
	vars->i++;
}
