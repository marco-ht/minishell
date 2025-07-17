/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards1b.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierant & sfelici <marvin@student.42ro    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 22:47:39 by mpierant &        #+#    #+#             */
/*   Updated: 2025/07/17 22:53:45 by mpierant &       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	init_expand_vars(t_execcmd *ecmd, t_expand_vars *vars)
{
	vars->argc = 0;
	while (ecmd->argv[vars->argc] && vars->argc < MAX_ARGS)
		vars->argc++;
	vars->i = 0;
	while (vars->i < vars->argc)
	{
		vars->old_argv[vars->i] = ecmd->argv[vars->i];
		vars->old_q[vars->i] = ecmd->qtype[vars->i];
		vars->old_alloc[vars->i] = ecmd->allocated[vars->i];
		vars->i++;
	}
	vars->i = 0;
	vars->t_count = 0;
}

int	handle_non_expandable_arg(t_execcmd *ecmd, t_expand_vars *vars)
{
	if (vars->old_q[vars->i] != 'a')
	{
		ecmd->argv[vars->t_count] = vars->old_argv[vars->i];
		ecmd->qtype[vars->t_count] = vars->old_q[vars->i];
		ecmd->allocated[vars->t_count] = vars->old_alloc[vars->i];
		vars->t_count++;
		vars->i++;
		return (1);
	}
	return (0);
}

int	handle_dot_slash_wildcard(t_execcmd *ecmd, t_expand_vars *vars)
{
	if (strncmp(vars->pat, "./", 2) == 0 && ft_strchr(vars->pat, '*'))
	{
		vars->found = handle_dot_slash_pattern(vars->pat, ecmd, &vars->t_count);
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

int	handle_hidden_files_wildcard(t_execcmd *ecmd, t_expand_vars *vars)
{
	if (vars->pat[0] == '.' && ft_strchr(vars->pat, '*'))
	{
		vars->found = handle_hidden_files(vars->pat, ecmd, &vars->t_count);
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

void	add_directory_matches_to_argv(t_execcmd *ecmd, t_expand_vars *vars)
{
	if (vars->match_count > 0)
	{
		sort_matches(vars->matches, vars->match_count);
		vars->i = 0;
		while (vars->i < vars->match_count && vars->t_count < MAX_ARGS - 1)
		{
			ecmd->argv[vars->t_count] = vars->matches[vars->i];
			ecmd->qtype[vars->t_count] = 'a';
			ecmd->allocated[vars->t_count] = 1;
			vars->t_count++;
			vars->i++;
		}
	}
}
