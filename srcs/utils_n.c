/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_n.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierant & sfelici <marvin@student.42ro    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 10:46:39 by mpierant          #+#    #+#             */
/*   Updated: 2025/07/23 00:25:43 by mpierant &       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_isnumreal(const char *s)
{
	size_t	i;

	i = 0;
	if (s == NULL || *s == '\0')
		return (0);
	if (s[0] == '+' || s[0] == '-')
	{
		if (s[1] == '\0')
			return (0);
		i = 1;
	}
	while (s[i] != '\0')
	{
		if (!ft_isdigit((unsigned char)s[i]))
			return (0);
		i++;
	}
	return (1);
}

int	ft_getcmd(char **buf)
{
	*buf = readline("$ ");
	if (!(*buf))
		return (-1);
	return (ft_strlen(*buf));
}

void	ft_exit_err(char *str)
{
	printf("%s\n", str);
	exit(1);
}

void	ft_exit_err_n(char *str, int *p_last_exit_status, int exit_code,
		t_vars *v)
{
	if (str)
	{
		ft_putstr_fd(str, 2);
		if (!ft_strchr(str, '\n'))
			ft_putstr_fd("\n", 2);
	}
	rl_clear_history();
	free(v->buf);
	free_tree(v->tree);
	ft_free_envp(v->envp);
	update_exit_status(exit_code, p_last_exit_status);
	exit(exit_code);
}

void	ft_ret_err(char *str)
{
	printf("%s\n", str);
	return ;
}
