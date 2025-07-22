/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins3_n.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierant & sfelici <marvin@student.42ro    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 11:42:20 by mpierant          #+#    #+#             */
/*   Updated: 2025/07/22 16:18:24 by mpierant &       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	builtin_exit(t_execcmd *ecmd, t_vars *v)
{
	int	exit_status;

	if (ecmd->argv[1] == NULL)
	{
		rl_clear_history();
		ft_exit_err_n(NULL, &v->last_exit_status, 0, v);
		//exit(0);
	}
	else if (ecmd->argv[2] != NULL)
	{
		ft_putstr_fd(" too many arguments\n", 2);
		return (1);
	}
	else if (!ft_isnumreal(ecmd->argv[1]))
	{
		//ft_putstr_fd(" numeric argument required\n", 2);
		ft_exit_err_n(" numeric argument required\n", &v->last_exit_status, 2, v);
		//exit(2);
	}
	else
	{
		exit_status = (int)(ft_atoi(ecmd->argv[1]) & 0xFF);
		rl_clear_history();
		ft_exit_err_n(NULL, &v->last_exit_status, exit_status, v);
		//exit(exit_status);
	}
	return(exit_status);
}

int	builtin_env(char **envp)
{
	int	i;

	i = 0;
	while (envp && envp[i])
	{
		printf("%s\n", envp[i]);
		i++;
	}
	return (0);
}

char	**find_env_var(char **envp, const char *key)
{
	int	key_len;

	key_len = ft_strlen(key);
	while (*envp)
	{
		if (ft_strncmp(*envp, key, key_len) == 0 && ((*envp)[key_len] == '='))
			return (envp);
		envp++;
	}
	return (NULL);
}

int	builtin_unset(t_execcmd *ecmd, char ***envp)
{
	char	**var_ptr;
	char	**next_var;
	int		i;

	if (!ecmd->argv[1])
		return (0);
	i = 1;
	while (ecmd->argv[i])
	{
		var_ptr = find_env_var(*envp, ecmd->argv[i]);
		if (var_ptr)
		{
			free(*var_ptr);
			next_var = var_ptr + 1;
			while (*next_var)
			{
				*var_ptr = *next_var;
				var_ptr++;
				next_var++;
			}
			*var_ptr = NULL;
		}
		i++;
	}
	return (0);
}
