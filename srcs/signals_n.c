/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_n.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierant & sfelici <marvin@student.42ro    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 10:54:31 by mpierant          #+#    #+#             */
/*   Updated: 2025/07/16 14:00:13 by mpierant &       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

volatile sig_atomic_t	g_signal_received = 0;

void	handler_interactive(int signum)
{
	g_signal_received = signum;
	if (signum == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	handler_child(int signum)
{
	g_signal_received = signum;
}

void	setup_signals_interactive(void)
{
	signal(SIGINT, handler_interactive);
	signal(SIGQUIT, SIG_IGN);
}

void	setup_signals_child(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

void	reset_signal_flag(void)
{
	g_signal_received = 0;
}
