/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierant <mpierant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 11:47:47 by mpierant          #+#    #+#             */
/*   Updated: 2025/07/16 11:47:48 by mpierant         ###   ########.fr       */
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
	if (!ecmd->argv[1])
	{
		printf("\n");
		return (0);
	}
	if (check_echo_args(ecmd->argv[1]))
	{
		if (!ecmd->argv[2])
			return (0);
		print_args_without_newline(ecmd->argv);
	}
	else
		print_args_with_newline(ecmd->argv);
	return (0);
}
