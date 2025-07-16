/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierant & sfelici <marvin@student.42ro    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 19:37:24 by mpierant &        #+#    #+#             */
/*   Updated: 2025/07/16 19:38:34 by mpierant &       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	init_vars(t_vars *v)
{
	v->buf = NULL;
	v->tmp = NULL;
	v->more = NULL;
	v->lineparser = NULL;
	v->status = 0;
	v->last_exit_status = 0;
	v->envp = NULL;
	v->tree = NULL;
}

int	parse_until_tree(t_vars *v)
{
	while (1)
	{
		v->lineparser = ft_strdup(v->buf);
		v->tree = ft_parsecmd(v->lineparser, &v->status);
		if (v->tree == NULL && v->status == 1)
		{
			free(v->lineparser);
			v->more = readline("> ");
			if (v->more == NULL)
				return (1);
			v->buf = ft_strjoin(v->buf, "\n");
			v->buf = ft_strjoin(v->buf, v->more);
			free(v->more);
			continue ;
		}
		break ;
	}
	if (v->tree == NULL)
	{
		free(v->lineparser);
		return (1);
	}
	return (0);
}

void	sanitize_buf(t_vars *v)
{
	v->tmp = v->buf;
	while (*v->tmp)
	{
		if (*v->tmp == '\n')
			*v->tmp = ' ';
		v->tmp++;
	}
}

void	execute_and_cleanup(t_vars *v)
{
	add_history(v->buf);
	ft_runcmd(v->tree, &v->envp, &v->last_exit_status);
	free(v->lineparser);
	free_tree(v->tree);
	free(v->buf);
}

void	read_and_parse(t_vars *v)
{
	while (ft_getcmd(&v->buf) >= 0)
	{
		reset_signal_flag();
		if (parse_until_tree(v))
			continue ;
		sanitize_buf(v);
		execute_and_cleanup(v);
	}
}
