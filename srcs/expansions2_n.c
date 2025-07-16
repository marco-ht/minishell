/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierant & sfelici <marvin@student.42ro    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 16:44:05 by mpierant &        #+#    #+#             */
/*   Updated: 2025/07/16 16:44:17 by mpierant &       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*get_env_value(const char *key, char **envp)
{
	int	key_len;

	if (!key || !envp)
		return ("");
	key_len = ft_strlen(key);
	while (*envp)
	{
		if (ft_strncmp(*envp, key, key_len) == 0 && (*envp)[key_len] == '=')
			return (&(*envp)[key_len + 1]);
		envp++;
	}
	return ("");
}

void	extract_var_name(char **arg, char *var_name)
{
	int	j;

	ft_bzero(var_name, 256);
	j = 0;
	while (ft_isalnum(**arg) || **arg == '_')
		var_name[j++] = *(*arg)++;
}

void	process_variable(char **arg, char *buffer, int *i, char **envp)
{
	char	var_name[256];
	char	*var_value;

	(*arg)++;
	extract_var_name(arg, var_name);
	var_value = get_env_value(var_name, envp);
	ft_strlcat(buffer, var_value, 4096);
	*i = ft_strlen(buffer);
}

void	perform_expansion(char **arg_ptr, char **envp)
{
	char	buffer[4096];
	char	*arg;
	int		i;

	arg = *arg_ptr;
	ft_bzero(buffer, sizeof(buffer));
	i = 0;
	while (*arg)
	{
		if (*arg == '$' && (ft_isalpha(*(arg + 1)) || *(arg + 1) == '_'))
			process_variable(&arg, buffer, &i, envp);
		else
			buffer[i++] = *arg++;
	}
	free(*arg_ptr);
	*arg_ptr = ft_strdup(buffer);
}

void	handle_exit_status_expansion(t_execcmd *ecmd, int i,
		int *p_last_exit_status)
{
	char	*expanded_exit;

	expanded_exit = expand_exit_status(ecmd->argv[i], p_last_exit_status);
	if (expanded_exit != ecmd->argv[i])
	{
		if (ecmd->allocated[i])
			free(ecmd->argv[i]);
		ecmd->argv[i] = expanded_exit;
		ecmd->allocated[i] = 1;
	}
}
