/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierant <mpierant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 10:33:46 by mpierant          #+#    #+#             */
/*   Updated: 2025/07/16 11:54:51 by mpierant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

// AST NODES TYPES CODES
# define EXEC 1
# define REDIR 2
# define PIPE 3
# define EXECP 5
# define AND 6
# define OR 7
# define HEREDOC 8

# define MAX_ARGS 1000

// AST NODES STRUCTS
typedef struct s_cmd
{
	int							type;
}								t_cmd;

typedef struct s_execcmd
{
	int							type;
	char						*argv[MAX_ARGS];
	char						*eargv[MAX_ARGS];
	char						qtype[MAX_ARGS];
	int							allocated[MAX_ARGS];
}								t_execcmd;

typedef struct s_redircmd
{
	int							type;
	t_cmd						*cmd;
	char						*file;
	char						*efile;
	int							mode;
	int							fd;
	int							allocated;
}								t_redircmd;

typedef struct s_pipecmd
{
	int							type;
	t_cmd						*left;
	t_cmd						*right;
}								t_pipecmd;

typedef struct s_andcmd
{
	int							type;
	t_cmd						*left;
	t_cmd						*right;
}								t_andcmd;

typedef struct s_orcmd
{
	int							type;
	t_cmd						*left;
	t_cmd						*right;
}								t_orcmd;

typedef struct s_heredoccmd
{
	int							type;
	t_cmd						*cmd;
	char						*lim_start;
	char						*lim_end;
	int							expand;
}								t_heredoccmd;

// GET CMD STRING FUNCTIONS
int								ft_getcmd(char **buf);

// ENV HANDLING
char							**ft_envcpy(char **env);

// EXECUTION
int								ft_runcmd(t_cmd *cmd, char ***envp,
									int *p_last_exit_status);
char							*find_path(char **cmd, char **envp);
void							ft_child(int *fd, t_heredoccmd *hcmd,
									char **envp);
char							*my_getenv(char **envp, char *word);

// PARSING
t_cmd							*ft_parsecmd(char *s, int *status);
t_cmd							*parseandor(char **ps, char *es);
t_cmd							*parsepipe(char **ps, char *es);
t_cmd							*parseredirs(t_cmd *cmd, char **ps, char *es);
t_cmd							*parseexec(char **ps, char *es);
t_cmd							*nulterminate(t_cmd *cmd);
int								gettoken(char **ps, char *es, char **q,
									char **eq);
int								peek(char **ps, char *es, char *toks);

// AST NODE'S CONSTRUCTORS
t_cmd							*ft_execcmd(void);
t_cmd							*ft_pipecmd(t_cmd *left, t_cmd *right);
t_cmd							*ft_redircmd(t_cmd *subcmd, char *file,
									char *efile, int mode, int fd,
									int allocated);
t_cmd							*ft_andcmd(t_cmd *left, t_cmd *right);
t_cmd							*ft_orcmd(t_cmd *left, t_cmd *right);
t_cmd							*ft_heredoccmd(t_cmd *subcmd, int tok_type,
									char *lim_start, char *lim_end);

// ERROR HANDLING
void							ft_exit_err(char *str);
void							ft_exit_err_n(char *str,
									int *p_last_exit_status);
void							ft_ret_err(char *str);
int								fork1(void);

// CLEANUP FUNCTIONS (No Leaks)
void							free_tree(t_cmd *cmd);
void							ft_free_envp(char **envp);

// BUILTINS
char							*get_pwd(void);
int								builtin_pwd(void);
int								builtin_echo(t_execcmd *ecmd);
int								builtin_cd(t_execcmd *ecmd, char **envp);
int								builtin_exit(t_execcmd *ecmd);
char							*get_current_directory_string(char *buffer,
									size_t size);
int								builtin_env(char **envp);
char							**find_env_var(char **envp, const char *key);
int								builtin_export(t_execcmd *ecmd, char ***envp);
int								builtin_unset(t_execcmd *ecmd, char ***envp);

// SIGNALS HANDLING
extern volatile sig_atomic_t	g_signal_received;

void							setup_signals_interactive(void);
void							setup_signals_child(void);
void							reset_signal_flag(void);
int								ft_isnumreal(const char *s);

// GNL
char							*get_next_line(int fd);
void							*ft_calloc(size_t nmemb, size_t size);
char							*ft_strchr(const char *s, int c);
char							*ft_strjoin(char const *s1, char const *s2);
size_t							ft_strlen(const char *s);

// AMBIENT VARIABLES
void							expand_variables(t_execcmd *ecmd, char **envp,
									int *p_last_exit_status);
void							update_exit_status(int status,
									int *p_last_exit_status);
char							*expand_exit_status(char *str,
									int *p_last_exit_status);
int								expand_wildcards(t_execcmd *ecmd);
void							perform_expansion(char **arg_ptr, char **envp);
int								match_star(const char *pat, const char *name);
char							*expand_redirect_glob(char *pattern,
									int *allocated);

#endif
