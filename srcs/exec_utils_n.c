/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils_n.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierant & sfelici <marvin@student.42ro    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 11:15:21 by mpierant          #+#    #+#             */
/*   Updated: 2025/07/16 13:59:49 by mpierant &       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_free_args(char **args)
{
	int	i;

	i = 0;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}

char	*my_getenv(char **envp, char *word)
{
	int	word_len;
	int	i;

	word_len = ft_strlen(word);
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], word, word_len) == 0)
			return (envp[i] + word_len);
		i++;
	}
	perror("PATH not found in environment");
	exit(1);
}

static char	*check_absolute_path(char *cmd)
{
	if (access(cmd, F_OK) == 0)
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		ft_putstr_fd(" Permission denied\n", 2);
		exit(126);
	}
	ft_putstr_fd(" No such file or directory\n", 2);
	exit(127);
}

static char	*search_in_paths(char **paths, char *cmd)
{
	char	*path_str;
	char	*cmd_str;
	int		i;

	i = 0;
	while (paths[i])
	{
		path_str = ft_strjoin(paths[i], "/");
		cmd_str = ft_strjoin(path_str, cmd);
		free(path_str);
		if (access(cmd_str, F_OK | X_OK) == 0)
		{
			ft_free_args(paths);
			return (cmd_str);
		}
		free(cmd_str);
		i++;
	}
	ft_free_args(paths);
	ft_putstr_fd(" command not found\n", 2);
	free(cmd);
	exit(127);
}

char	*find_path(char **cmd, char **envp)
{
	char	*path_str;
	char	**paths;

	if (ft_strchr(cmd[0], '/'))
		return (check_absolute_path(cmd[0]));
	path_str = my_getenv(envp, "PATH=");
	paths = ft_split(path_str, ':');
	return (search_in_paths(paths, cmd[0]));
}
