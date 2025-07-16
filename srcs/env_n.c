/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierant & sfelici <marvin@student.42ro    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 14:32:31 by mpierant &        #+#    #+#             */
/*   Updated: 2025/07/16 14:33:51 by mpierant &       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_free_envp(char **envp)
{
	int	i;

	if (!envp)
		return ;
	i = -1;
	while (envp[++i])
		free(envp[i]);
	free(envp);
}

static int	count_env(char **env)
{
	int	i;

	i = 0;
	while (env[i])
		i++;
	return (i);
}

static void	free_partial_envp(char **envp, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		free(envp[i]);
		i++;
	}
	free(envp);
}

char	**ft_envcpy(char **env)
{
	int		i;
	int		count;
	char	**envp;

	count = count_env(env);
	envp = malloc((count + 1) * sizeof(char *));
	if (!envp)
	{
		perror("envcpy");
		exit(1);
	}
	i = 0;
	while (i < count)
	{
		envp[i] = ft_strdup(env[i]);
		if (!envp[i])
		{
			free_partial_envp(envp, i);
			return (NULL);
		}
		i++;
	}
	envp[count] = NULL;
	return (envp);
}
