/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser6_n.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierant & sfelici <marvin@student.42ro    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 00:08:11 by mpierant &        #+#    #+#             */
/*   Updated: 2025/07/19 00:30:37 by mpierant &       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	add_argument(t_parseexec_vars *vars)
{
	vars->cmd->argv[vars->argc] = vars->q;
	vars->cmd->eargv[vars->argc] = vars->eq;
	vars->cmd->qtype[vars->argc] = vars->tok;
	vars->argc++;
	if (vars->argc >= MAX_ARGS)
	{
		printf("too many args");
		free(vars->cmd);
		free(vars->ret);
		vars->ret = NULL;
	}
}

static int	parseexec_loop(t_parseexec_vars *vars, char **ps, char *es)
{
	vars->argc = 0;
	vars->ret = parseredirs(vars->ret, ps, es);
	while (1)
	{
		vars->backup = *ps;
		vars->tok = gettoken(ps, es, &vars->q, &vars->eq);
		if (vars->tok != 'a' && vars->tok != 's' && vars->tok != 'd')
		{
			*ps = vars->backup;
			break ;
		}
		add_argument(vars);
		if (!vars->ret)
			return (0);
		vars->ret = parseredirs(vars->ret, ps, es);
	}
	vars->cmd->argv[vars->argc] = NULL;
	vars->cmd->eargv[vars->argc] = NULL;
	return (1);
}

t_cmd	*parseexec(char **ps, char *es)
{
	t_parseexec_vars	vars;

	if (peek(ps, es, "("))
		return (parseblock(ps, es));
	vars.ret = ft_execcmd();
	vars.cmd = (t_execcmd *)vars.ret;
	if (!parseexec_loop(&vars, ps, es))
		return (NULL);
	return (vars.ret);
}
