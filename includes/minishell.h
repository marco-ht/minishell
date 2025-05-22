#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>
# include <limits.h>

# define EXEC 1
# define REDIR 2
# define PIPE 3
# define LIST 4
# define BUILTIN 5

# define MAX_ARGS 10

typedef struct s_cmd
{
	int		type;
}			t_cmd;

typedef struct s_execcmd
{
	int		type;
	char	*argv[MAX_ARGS];
	char	*eargv[MAX_ARGS];
}			t_execcmd;

typedef struct s_redircmd
{
	int		type;
	t_cmd	*cmd;
	char	*file;
	char	*efile;
	int		mode;
	int		fd;
}			t_redircmd;

typedef struct s_pipecmd
{
	int		type;
	t_cmd	*left;
	t_cmd	*right;
}			t_pipecmd;

typedef struct s_listcmd
{
	int		type;
	t_cmd	*left;
	t_cmd	*right;
}			t_listcmd;

int			ft_getcmd(char **buf);
void		ft_runcmd(t_cmd *cmd);

t_cmd		*ft_execcmd(void);
t_cmd		*ft_execbuiltin(void);
t_cmd		*ft_pipecmd(t_cmd *left, t_cmd *right);
t_cmd		*ft_redircmd(t_cmd *subcmd, char *file, char *efile, int mode,
				int fd);
t_cmd		*ft_listcmd(t_cmd *left, t_cmd *right);

void		ft_exit_err(char *str);
int			fork1(void);

t_cmd		*ft_tree_test(void);
void		ft_ret_err(char *str);
t_cmd		*ft_tree_pipe2(void);
t_cmd		*ft_tree_pipe3(void);
t_cmd		*ft_tree_builtin(void);

void	builtin_pwd(t_execcmd *ecmd);

#endif
