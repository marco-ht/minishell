/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins4_n.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierant & sfelici <marvin@student.42ro    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 11:52:15 by mpierant          #+#    #+#             */
/*   Updated: 2025/07/22 06:07:36 by mpierant &       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	handle_home_cd(char *home)
{
	if (!home || chdir(home) < 0)
	{
		perror("minishell: cd");
		return (1);
	}
	return (0);
}

int	handle_tilde_subpath(char *home, char *subpath)
{
	char	*joined;

	joined = ft_strjoin(home, subpath);
	if (!joined || chdir(joined) < 0)
	{
		perror("minishell: cd");
		if (joined)
			free(joined);
		return (1);
	}
	free(joined);
	return (0);
}

int	handle_tilde_cd(char *arg, char **envp)
{
	char	*home;
	char	*subpath;

	home = my_getenv(envp, "HOME=");
	if (!home)
	{
		perror("minishell: cd");
		return (1);
	}
	if (arg[1] == '\0')
		return (handle_home_cd(home));
	if (arg[1] == '/')
	{
		subpath = arg + 1;
		return (handle_tilde_subpath(home, subpath));
	}
	perror("minishell: cd");
	return (1);
}

int	builtin_cd(t_execcmd *ecmd, char **envp)
{
	char	*home;

	if (ecmd->argv[1] && ecmd->argv[2])
	{
		ft_putstr_fd(" too many arguments\n", 2);
		return (1);
	}
	if (!ecmd->argv[1])
	{
		home = my_getenv(envp, "HOME=");
		return (handle_home_cd(home));
	}
	if (ecmd->argv[1][0] == '~')
		return (handle_tilde_cd(ecmd->argv[1], envp));
	if (chdir(ecmd->argv[1]) < 0)
	{
		perror("minishell: cd");
		return (1);
	}
	return (0);
}
