/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_n.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierant & sfelici <marvin@student.42ro    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 19:36:11 by mpierant &        #+#    #+#             */
/*   Updated: 2025/07/23 00:48:41 by mpierant &       ###   ########.fr       */
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
	free(v.buf);
	ft_free_envp(v.envp);
	rl_clear_history();
	return (0);
}

/*

make -C .. re && clear && ./tester && ./tester wildcards


valgrind   --leak-check=full   --show-leak-kinds=all   --track-origins=yes
   --suppressions=supp.supp   --log-file=valgrind-minishell.log
   ./minishell < test.cmds


valgrind   --leak-check=full   --show-leak-kinds=all   --track-origins=yes
   --suppressions=supp.supp   ./minishell

*/

//		supp.supp
//{
//    ignore_libreadline_leaks
//    Memcheck:Leak
//    ...
//    obj:*/libreadline.so.*
//}
