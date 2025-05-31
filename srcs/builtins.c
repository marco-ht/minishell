#include "../includes/minishell.h"

char *get_pwd()
{
	char *cwd = getcwd(NULL, 0);
	if (!cwd)
		perror("minishell: cd");
	return (cwd);
}

void builtin_pwd()
{
	char *cwd;

	cwd = get_pwd();
	if (cwd)
	{
		printf("%s\n", cwd);
		free(cwd);
	}
}
int check_echo_args(char *arg)
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

void builtin_echo(t_execcmd *ecmd)
{
	int i;

	if (!ecmd->argv[1])
	{
		printf("\n");
		return;
	}
	if (check_echo_args(ecmd->argv[1]))
	{
		i = 2;
		if (!ecmd->argv[3])
		{
			printf("%s", ecmd->argv[2]);
		}
		else if (ecmd->argv[3])
		{
			while (ecmd->argv[i])
			{
				printf("%s", ecmd->argv[i++]);
				if (ecmd->argv[i])
					printf(" ");
				else
					return;
			}
		}
		else
		{
			perror("minishell: echo");
		}
	}
	else
	{
		i = 1;
		if (!ecmd->argv[2])
		{
			printf("%s\n", ecmd->argv[1]);
		}
		else if (ecmd->argv[2])
		{
			while (ecmd->argv[i])
			{
				printf("%s", ecmd->argv[i++]);
				if (ecmd->argv[i])
					printf(" ");
				else
					printf("\n");
			}
		}
		else
		{
			perror("minishell: echo");
		}
	}
}

void builtin_cd(t_execcmd *ecmd)
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
		perror("minishell: exit");
		return ;
	}
	else if (!ft_isnumreal(ecmd->argv[1]))
	{
		perror("minishell: exit");
		exit(255);
	}
	else
	{
		exit_status = (int)(ft_atoi(ecmd->argv[1]) & 0xFF);
		rl_clear_history();
		exit(exit_status);
	}
	return ;
}
