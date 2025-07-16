/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins7.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierant <mpierant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 13:28:01 by mpierant          #+#    #+#             */
/*   Updated: 2025/07/16 13:28:31 by mpierant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	add_new_var(char ***envp, char *arg_copy, char *value)
{
	char	**new_envp;
	int		count;

	count = count_env_vars(*envp);
	new_envp = ft_calloc(count + 2, sizeof(char *));
	if (!new_envp)
	{
		perror("minishell: export");
		return (1);
	}
	copy_env_vars(new_envp, *envp, count);
	*(value - 1) = '=';
	new_envp[count] = ft_strdup(arg_copy);
	*(value - 1) = '\0';
	if (!new_envp[count])
	{
		perror("minishell: export");
		free(new_envp);
		return (1);
	}
	new_envp[count + 1] = NULL;
	free(*envp);
	*envp = new_envp;
	return (0);
}

int	handle_var_assignment(char *key, char *value, char ***envp, char *arg_copy)
{
	char	**var_ptr;

	var_ptr = find_env_var(*envp, key);
	if (var_ptr)
	{
		if (update_existing_var(var_ptr, arg_copy, value))
			return (1);
	}
	else
	{
		if (add_new_var(envp, arg_copy, value))
			return (1);
	}
	return (0);
}

int	validate_and_process_key(char *key, char *value, char ***envp,
		char *arg_copy)
{
	if (!is_valid_identifier(key))
	{
		ft_putstr_fd(" not a valid identifier\n", 2);
		free(arg_copy);
		return (1);
	}
	if (handle_var_assignment(key, value, envp, arg_copy))
	{
		free(arg_copy);
		return (1);
	}
	return (0);
}

int	handle_no_value_case(char *key, char *arg_copy, char ***envp)
{
	char	*value;
	char	**var_ptr;

	if (!is_valid_identifier(key))
	{
		ft_putstr_fd(" not a valid identifier\n", 2);
		free(arg_copy);
		return (1);
	}
	var_ptr = find_env_var(*envp, key);
	if (!var_ptr)
	{
		value = key + ft_strlen(key);
		*value = '=';
		*(value + 1) = '\0';
		if (add_new_var(envp, arg_copy, value + 1))
			return (1);
		*value = '\0';
	}
	return (0);
}

int	process_export_arg(char *arg, char ***envp)
{
	char	*arg_copy;
	char	*key;
	char	*value;

	arg_copy = ft_strdup(arg);
	if (!arg_copy)
		return (perror("minishell: export"), 1);
	key = arg_copy;
	value = ft_strchr(key, '=');
	if (value)
	{
		*value = '\0';
		value++;
		if (validate_and_process_key(key, value, envp, arg_copy))
			return (1);
	}
	else
	{
		if (handle_no_value_case(key, arg_copy, envp))
			return (1);
	}
	free(arg_copy);
	return (0);
}
