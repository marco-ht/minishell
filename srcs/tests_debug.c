#include "../includes/minishell.h"

void	ft_check(t_exec *exec_str)
{
	int i;

	i = 0;
	while(exec_str->evargs[i])
	{
		printf("*exec_str->evargs[%d] =%d ", i, is_whitespace(*(exec_str->evargs[i])));
		i++;
	}
	printf("\n");
	i =0;
	while(exec_str->evargs[i])
	{
		printf("exec_str->evargs[%d] =%p ", i, (exec_str->evargs[i]));
		i++;
	}
	printf("\n");
}


void	ft_check_args(t_exec *exec_str)
{
	int i;

	printf("\n");
	printf("\n");
	i = 0;
	while(exec_str->args[i])
	{
		printf("*exec_str->args[%d] =%d ", i, is_whitespace(*(exec_str->args[i])));
		i++;
	}
	printf("\n");
	i =0;
	while(exec_str->args[i])
	{
		printf("exec_str->args[%d] =%p ", i, (exec_str->args[i]));
		i++;
	}
	printf("\n");
}

/* void	set_zeros(char *line, t_exec *exec_str)
{
	int	i;
	int	j;

	exec_str->evargs = malloc(100 * sizeof(char *));

	printf("line start = %p ", line);
	printf("\n");
	j = 0;
	while (line[j] && is_whitespace(line[j]))
		j++;
	i = 0;
	while(line[j])
	{
		if (!ft_strchr(&line[j], ' '))
		{
			exec_str->evargs[i] = &line[j];
			i++;
			while (line[j] && is_whitespace(line[j]))
				j++;
		}
		else
			j++;
	}
	exec_str->evargs[i] = NULL;
	ft_check(exec_str);
} */
