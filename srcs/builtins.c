#include "../includes/minishell.h"

char	*get_pwd()
{
	char *cwd = getcwd(NULL, 0);
	if (!cwd)
		perror("minishell: cd");
	return (cwd);
}

void	builtin_pwd()
{
	char *cwd;

	cwd = get_pwd();
	if (cwd)
	{
		printf("%s\n", cwd);
		free(cwd);
	}
}
int	check_echo_args(char *arg)
{
	if (!arg)
		return (0);
	if (arg[0] != '-')
		return (0);
	int i = 1;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

void	builtin_echo(t_execcmd *ecmd)
{
	int	i;

	if (!ecmd->argv[1])
	{
		printf("\n");
		return;
	}
	if (check_echo_args(ecmd->argv[1]))
	{
		i = 2;
		if (!ecmd->argv[2])
			return;
		while (ecmd->argv[i])
		{
			printf("%s", ecmd->argv[i++]);
			if (ecmd->argv[i])
				printf(" ");
		}
	}
	else
	{
		i = 1;
		while (ecmd->argv[i])
		{
			printf("%s", ecmd->argv[i++]);
			if (ecmd->argv[i])
				printf(" ");
		}
		printf("\n");
	}
}

void	builtin_cd(t_execcmd *ecmd)
{
	char *home;
	char *subpath;
	char *joined;

	if (!ecmd->argv[1])
	{
		home = getenv("HOME");
		if (!home || chdir(home) < 0)
			perror("minishell: cd");
	}
	else if (ecmd->argv[1][0] == '~')
	{
		home = getenv("HOME");
		if (!home)
		{
			perror("minishell: cd");
			return;
		}
		if (ecmd->argv[1][1] == '\0')
		{
			if (chdir(home) < 0)
				perror("minishell: cd");
		}
		else if (ecmd->argv[1][1] == '/')
		{
			subpath = ecmd->argv[1] + 1;
			joined = ft_strjoin(home, subpath);

			if (!joined || chdir(joined) < 0)
				perror("minishell: cd");

			free(joined);
		}
		else
		{
			perror("minishell: cd");
		}
	}
	else
	{
		if (chdir(ecmd->argv[1]) < 0)
			perror("minishell: cd");
	}
}

void	builtin_exit(t_execcmd *ecmd)
{
	int		exit_status;

	if (ecmd->argv[1] == NULL)
	{
		rl_clear_history();
		exit(0);
	}
	else if (ecmd->argv[2] != NULL)
	{
		perror("minishell: exit: too many arguments");
		return ;
	}
	else if (!ft_isnumreal(ecmd->argv[1]))
	{
		perror("minishell: exit: numeric argument required");
		exit(255);
	}
	else
	{
		exit_status = (int)(ft_atoi(ecmd->argv[1]) & 0xFF);
		rl_clear_history();
		exit(exit_status);
	}
}

void    builtin_env(char **envp)
{
	int i;

	i = 0;
	while (envp && envp[i])
	{
		printf("%s\n", envp[i]);
		i++;
	}
}

static char	**find_env_var(char **envp, const char *key)
{
	int key_len;

	key_len = ft_strlen(key);
	while (*envp)
	{
		if (ft_strncmp(*envp, key, key_len) == 0 && ((*envp)[key_len] == '='))
		{
			return (envp);
		}
		envp++;
	}
	return (NULL);
}

void	builtin_export(t_execcmd *ecmd, char ***envp)
{
	char	*arg_copy;
	char	*key;
	char	*value;
	char	**var_ptr;
	char	**new_envp;
	int		i;
	int		j;
	int		count;

	if (!ecmd->argv[1])
	{
		i = 0;
		while ((*envp)[i])
		{
			printf("declare -x %s\n", (*envp)[i]);
			i++;
		}
		return ;
	}
	i = 1;
	while (ecmd->argv[i])
	{
		arg_copy = ft_strdup(ecmd->argv[i]);
		if (!arg_copy)
		{
			perror("minishell: export");
			return ;
		}
		key = arg_copy;
		value = ft_strchr(key, '=');
		if (value)
		{
			*value = '\0';
			value++;
			var_ptr = find_env_var(*envp, key);
			if (var_ptr)
			{
				free(*var_ptr);
				*var_ptr = ft_strdup(ecmd->argv[i]);
			}
			else
			{
				count = 0;
				while ((*envp)[count])
					count++;
				new_envp = ft_calloc(count + 2, sizeof(char *));
				if (!new_envp)
				{
					perror("minishell: export");
					free(arg_copy);
					return ;
				}
				j = 0;
				while (j < count)
				{
					new_envp[j] = (*envp)[j];
					j++;
				}
				new_envp[count] = ft_strdup(ecmd->argv[i]);
				new_envp[count + 1] = NULL;
				free(*envp);
				*envp = new_envp;
			}
		}
		free(arg_copy);
		i++;
	}
}

void	builtin_unset(t_execcmd *ecmd, char ***envp)
{
	char	**var_ptr;
	char	**next_var;
	int		i;

	if (!ecmd->argv[1])
		return;
	
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
}
