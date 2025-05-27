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
		return ;
	}
	if (check_echo_args(ecmd->argv[1]))
	{
		i = 2;
		if(!ecmd->argv[3])
		{
			printf("%s", ecmd->argv[2]);
		}
		else if(ecmd->argv[3])
		{
			while (ecmd->argv[i])
			{
				printf("%s", ecmd->argv[i++]);
				if (ecmd->argv[i])
					printf(" ");
				else
					return ;
			}
		}
		else
		{
			perror("minishell: echo\n");
		}
	}
	else
	{
		i = 1;
		if(!ecmd->argv[2])
		{
			printf("%s\n", ecmd->argv[1]);
		}
		else if(ecmd->argv[2])
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
			perror("minishell: echo\n");
		}
	}
}

void	builtin_cd(t_execcmd *ecmd)
{
	if (!ecmd->argv[1])
		printf("no args\n");

	else if (ecmd->argv[1])
	{
		/* printf("pid cd = %d\n", getpid()); */
		/* printf("argv[1] = %s\n", ecmd->argv[1]); */
		if(chdir(ecmd->argv[1]) < 0)
		printf("cannot cd %s\n", ecmd->argv[1]);
	}
}
