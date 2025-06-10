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

char	*find_path(char **cmd)
{
	char	*path_str;
	char	*cmd_str;
	char	**paths;
	int		i;

	path_str = getenv("PATH");
	paths = ft_split(path_str, ':');
	i = 0;
	while (paths[i])
	{
		path_str = ft_strjoin(paths[i], "/");
		cmd_str = ft_strjoin(path_str, cmd[0]);
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
	perror("Command not found");
	exit(127);
}