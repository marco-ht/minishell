/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins5.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierant <mpierant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 11:52:19 by mpierant          #+#    #+#             */
/*   Updated: 2025/07/16 12:25:02 by mpierant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	is_valid_identifier(const char *str)
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
	int	i;

	i = 0;
	while (envp[i])
	{
		printf("declare -x %s\n", envp[i]);
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

int	handle_no_value_case(char *key, char *arg_copy)
{
	if (!is_valid_identifier(key))
	{
		ft_putstr_fd(" not a valid identifier\n", 2);
		free(arg_copy);
		return (1);
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
		if (handle_no_value_case(key, arg_copy))
			return (1);
	}
	free(arg_copy);
	return (0);
}

int	builtin_export(t_execcmd *ecmd, char ***envp)
{
	int	i;

	if (!ecmd->argv[1])
		return (print_all_env(*envp));
	i = 1;
	while (ecmd->argv[i])
	{
		if (process_export_arg(ecmd->argv[i], envp))
			return (1);
		i++;
	}
	return (0);
}
