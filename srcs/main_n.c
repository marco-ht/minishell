/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierant & sfelici <marvin@student.42ro    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 19:36:11 by mpierant &        #+#    #+#             */
/*   Updated: 2025/07/16 19:37:59 by mpierant &       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(int argc, char **argv, char **env)
{
	t_vars	v;

	init_vars(&v);
	if (argc > 0 && argv[0] != NULL)
		v.envp = ft_envcpy(env);
	setup_signals_interactive();
	read_and_parse(&v);
	free_tree(v.tree);
	free(v.buf);
	ft_free_envp(v.envp);
	rl_clear_history();
	return (0);
}
