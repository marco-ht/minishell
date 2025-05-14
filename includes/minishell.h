#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
# include <stdlib.h>
# include <unistd.h>

typedef struct	s_exec
{
	int		type;
	char	**args;
	char	**evargs;
}			t_exec;

typedef struct s_redirect
{
}			t_redirect;

typedef struct s_pipe
{
}			t_pipe;

void		set_zeros(char *line, t_exec *exec_str);
void		set_starts(char *line, t_exec *exec_str);
int			is_whitespace(char c);

// DEBUG FUNCTIONS
void		ft_check(t_exec *exec_str);
void		ft_check_args(t_exec *exec_str);

#endif