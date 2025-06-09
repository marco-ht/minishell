#include "../includes/minishell.h"

int	ft_isnumreal(const char *s)
{
	size_t	i;

	i = 0;
	if (s == NULL || *s == '\0')
		return (0);
	if (s[0] == '+' || s[0] == '-')
	{
		if (s[1] == '\0')
			return (0);
		i = 1;
	}
	while (s[i] != '\0')
	{
		if (!ft_isdigit((unsigned char)s[i]))
			return (0);
		i++;
	}
	return (1);
}

int ft_getcmd(char **buf)
{
	*buf = readline("$ ");
	if (!(*buf))
		return (-1); // EOF
	return (ft_strlen(*buf));
}

void ft_exit_err(char *str)
{
	printf("%s\n", str);
	exit(1);
}

void ft_ret_err(char *str)
{
	printf("%s\n", str);
	return;
}

int fork1(void)
{
	pid_t pid;

	pid = fork();
	if (pid == -1)
		ft_exit_err("Fork");
	return (pid);
}
