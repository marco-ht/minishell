/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierant & sfelici <marvin@student.42ro    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 21:55:43 by mpierant &        #+#    #+#             */
/*   Updated: 2025/07/17 23:04:29 by mpierant &       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	finalize_argv(t_execcmd *ecmd, t_expand_vars *vars)
{
	ecmd->argv[vars->t_count] = NULL;
	ecmd->qtype[vars->t_count] = '\0';
	ecmd->allocated[vars->t_count] = 0;
	vars->t_count++;
	while (vars->t_count < MAX_ARGS)
	{
		ecmd->argv[vars->t_count] = NULL;
		ecmd->qtype[vars->t_count] = '\0';
		ecmd->allocated[vars->t_count] = 0;
		vars->t_count++;
	}
}

int	expand_wildcards(t_execcmd *ecmd)
{
	t_expand_vars	vars;

	init_expand_vars(ecmd, &vars);
	while (vars.i < vars.argc && vars.t_count < MAX_ARGS - 1)
	{
		vars.pat = vars.old_argv[vars.i];
		if (handle_non_expandable_arg(ecmd, &vars))
			continue ;
		if (handle_dot_slash_wildcard(ecmd, &vars))
			continue ;
		if (handle_hidden_files_wildcard(ecmd, &vars))
			continue ;
		if (handle_slash_wildcard(ecmd, &vars))
			continue ;
		if (handle_simple_wildcard(ecmd, &vars))
			continue ;
		handle_regular_arg(ecmd, &vars);
	}
	finalize_argv(ecmd, &vars);
	return (0);
}
