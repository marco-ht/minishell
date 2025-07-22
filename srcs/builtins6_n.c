/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins6_n.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierant & sfelici <marvin@student.42ro    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 13:25:37 by mpierant          #+#    #+#             */
/*   Updated: 2025/07/16 13:59:31 by mpierant &       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_valid_identifier(const char *str)
{
	int	i;

	if (!str || !*str)
		return (0);
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	i = 1;
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int	print_all_env(char **envp)
{
	int		i;
	char	*equal_sign_pos;

	i = 0;
	while (envp && envp[i])
	{
		equal_sign_pos = ft_strchr(envp[i], '=');
		if (equal_sign_pos)
		{
			*equal_sign_pos = '\0';
			printf("declare -x %s=\"%s\"\n", envp[i], equal_sign_pos + 1);
			*equal_sign_pos = '=';
		}
		else
		{
			printf("declare -x %s\n", envp[i]);
		}
		i++;
	}
	return (0);
}

int	update_existing_var(char **var_ptr, char *arg_copy, char *value)
{
	free(*var_ptr);
	*(value - 1) = '=';
	*var_ptr = ft_strdup(arg_copy);
	*(value - 1) = '\0';
	if (!*var_ptr)
	{
		perror("minishell: export");
		return (1);
	}
	return (0);
}

int	count_env_vars(char **envp)
{
	int	count;

	count = 0;
	while (envp[count])
		count++;
	return (count);
}

int	copy_env_vars(char **new_envp, char **envp, int count)
{
	int	j;

	j = 0;
	while (j < count)
	{
		new_envp[j] = envp[j];
		j++;
	}
	return (0);
}
