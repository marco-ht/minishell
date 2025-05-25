#include "../includes/minishell.h"

void	builtin_pwd()
{
	char cwd_buffer[1024];

	if (getcwd(cwd_buffer, sizeof(cwd_buffer)) != NULL)
	{
		printf("%s\n", cwd_buffer);
	}
	else
	{
		perror("minishell: pwd\n");
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
