/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2_n.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierant & sfelici <marvin@student.42ro    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 10:48:34 by mpierant          #+#    #+#             */
/*   Updated: 2025/07/16 14:00:24 by mpierant &       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	fork1(void)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		ft_exit_err("Fork");
	return (pid);
}
