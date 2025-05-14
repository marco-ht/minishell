#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "../libft/libft.h"

int	ft_strcmp(const char *s1, const char *s2);

typedef struct	s_exec
{
	int		type;
	char	**args;
	char	**evargs;
} t_exec;

typedef struct	s_redirect
{

} t_redirect;

typedef struct	s_pipe
{

} t_pipe;

void    set_zeros(char *line, t_exec *exec_str);
void set_starts(char *line, t_exec *exec_str);

#endif