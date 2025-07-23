/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins2_n.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierant & sfelici <marvin@student.42ro    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 11:47:47 by mpierant          #+#    #+#             */
/*   Updated: 2025/07/16 13:59:06 by mpierant &       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	check_echo_args(char *arg)
{
	int	i;

	if (!arg)
		return (0);
	if (arg[0] != '-')
		return (0);
	i = 1;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

void	print_args_with_newline(char **argv)
{
	int	i;

	i = 1;
	while (argv[i])
	{
		if (i > 1)
			printf(" ");
		printf("%s", argv[i]);
		i++;
	}
	printf("\n");
}

void	print_args_without_newline(char **argv)
{
	int	i;

	i = 2;
	while (argv[i])
	{
		if (i > 2)
			printf(" ");
		printf("%s", argv[i]);
		i++;
	}
}

int	builtin_echo(t_execcmd *ecmd)
{
	int	i;
	int	newline;
	int	first_printed;

	i = 1;
	newline = 1;
	first_printed = 1;
	while (ecmd->argv[i] && check_echo_args(ecmd->argv[i]))
	{
		newline = 0;
		i++;
	}
	while (ecmd->argv[i])
	{
		if (!first_printed)
			printf(" ");
		printf("%s", ecmd->argv[i]);
		first_printed = 0;
		i++;
	}
	if (newline)
		printf("\n");
	return (0);
}
