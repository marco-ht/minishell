/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierant & sfelici <marvin@student.42ro    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 10:33:46 by mpierant          #+#    #+#             */
/*   Updated: 2025/07/22 16:03:49 by mpierant &       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <stdio.h>
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
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

// ft_redircmd INPUT STRUCT
typedef struct s_redin
{
	t_cmd						*subcmd;
	char						*file;
	char						*efile;
	int							mode;
	int							fd;
	int							allocated;
}								t_redin;

// parseexec VARS STRUCT
typedef struct s_parseexec_vars
{
	t_execcmd					*cmd;
	t_cmd						*ret;
	char						*q;
	char						*eq;
	char						*backup;
	int							tok;
	int							argc;
}								t_parseexec_vars;

// MAIN STRUCT
typedef struct s_vars
{
	char						*buf;
	char						*tmp;
	char						*more;
	char						*old_buf;
	char						*lineparser;
	int							status;
	int							last_exit_status;
	char						**envp;
	t_cmd						*tree;
}								t_vars;

// MAIN FUNCTIONS
void							init_vars(t_vars *v);
int								parse_until_tree(t_vars *v);
void							sanitize_buf(t_vars *v);
void							execute_and_cleanup(t_vars *v);
void							read_and_parse(t_vars *v);

// GET CMD STRING FUNCTIONS
int								ft_getcmd(char **buf);

// ENV HANDLING
char							**ft_envcpy(char **env);

// EXECUTION
int								ft_runcmd(t_cmd *cmd, char ***envp,
									int *p_last_exit_status, t_vars *v);
char							*find_path(char **cmd, char **envp);
void							ft_child(int *fd, t_heredoccmd *hcmd,
									char **envp);
char							*my_getenv(char **envp, char *word);
int								ft_check_builtin(t_execcmd *ecmd, char ***envp,
									int *p_last_exit_status, t_vars *v);
void							remove_empty_args(t_execcmd *ecmd);
int								apply_redirs(t_cmd *cmd,
									int *p_last_exit_status, t_vars *v);

// runcmd helpers
int								run_exec_cmd(t_cmd *cmd, char ***envp,
									int *p_last_exit_status, t_vars *v);
int								run_execp_cmd(t_cmd *cmd, char ***envp,
									int *p_last_exit_status, t_vars *v);
int								run_redir_cmd(t_cmd *cmd, char ***envp,
									int *p_last_exit_status, t_vars *v);
int								run_pipe_cmd(t_cmd *cmd, char ***envp,
									int *p_last_exit_status, t_vars *v);
int								run_and_cmd(t_cmd *cmd, char ***envp,
									int *p_last_exit_status, t_vars *v);
int								run_or_cmd(t_cmd *cmd, char ***envp,
									int *p_last_exit_status, t_vars *v);
int								run_heredoc_cmd(t_cmd *cmd, char ***envp,
									int *p_last_exit_status, t_vars *v);

// run_pipe_cmd vars struct
typedef struct s_pipe_ctx
{
	int							p[2];
	int							status_right;
	pid_t						pid_left;
	pid_t						pid_right;
	t_pipecmd					*pcmd;
}								t_pipe_ctx;

// run_redir_cmd vars struct
typedef struct s_redir_ctx
{
	t_redircmd					*redirs[32];
	int							n;
	t_cmd						*cur;
	int							saved_stdin;
	int							saved_stdout;
	int							status;
}								t_redir_ctx;

// PARSING: LEXER (gettoken) struct and helpers
typedef struct s_token_data
{
	char						*s;
	char						*es;
	char						**q;
	char						**eq;
	char						*whitespace;
	char						*symbols;
	int							ret;
}								t_token_data;

void							set_positions(t_token_data *data);
int								handle_pipe_token(t_token_data *data);
int								handle_and_token(t_token_data *data);
int								handle_redirect_out_token(t_token_data *data);
int								handle_redirect_in_token(t_token_data *data);
void							handle_quoted_string(t_token_data *data,
									char **out_write, char quote, int *ret);
void							handle_argument_loop(t_token_data *data,
									char **out_write, int *ret);
int								handle_argument_token(t_token_data *data);
int								dispatch_token(t_token_data *data);
void							skip_whitespace_update_ps(t_token_data *data,
									char **ps);

// PARSING: SYNTAX
t_cmd							*ft_parsecmd(char *s, int *status);
t_cmd							*parseandor(char **ps, char *es);
t_cmd							*parseblock(char **ps, char *es);
t_cmd							*parsepipe(char **ps, char *es);
t_cmd							*parseredirs(t_cmd *cmd, char **ps, char *es);
t_cmd							*parseexec(char **ps, char *es);
t_cmd							*nulterminate(t_cmd *cmd);
int								gettoken(char **ps, char *es, char **q,
									char **eq);
int								peek(char **ps, char *es, char *toks);

// NULLTERMINATE TREE helpers
void							handle_exec_cmd(t_cmd *cmd);
void							handle_redir_cmd(t_cmd *cmd);
void							handle_pipe_cmd(t_cmd *cmd);
void							handle_and_cmd(t_cmd *cmd);
void							handle_or_cmd(t_cmd *cmd);
void							handle_heredoc_cmd(t_cmd *cmd);

// AST NODE'S CONSTRUCTORS
t_cmd							*ft_execcmd(void);
t_cmd							*ft_pipecmd(t_cmd *left, t_cmd *right);
t_cmd							*ft_redircmd(t_redin *rin);
t_cmd							*ft_andcmd(t_cmd *left, t_cmd *right);
t_cmd							*ft_orcmd(t_cmd *left, t_cmd *right);
t_cmd							*ft_heredoccmd(t_cmd *subcmd, int tok_type,
									char *lim_start, char *lim_end);

// ERROR HANDLING
void							ft_exit_err(char *str);
void							ft_exit_err_n(char *str,
									int *p_last_exit_status, int exit_code,
									t_vars *v);
void							ft_ret_err(char *str);
int								fork1(void);

// CLEANUP FUNCTIONS (No Leaks)
void							free_tree(t_cmd *cmd);
void							free_exec(t_execcmd *ex);
void							free_pipe_cmd(t_pipecmd *pipecmd);
void							ft_free_envp(char **envp);

// BUILTINS
char							*get_pwd(void);
int								builtin_pwd(void);
int								builtin_echo(t_execcmd *ecmd);
int								builtin_cd(t_execcmd *ecmd, char **envp);
int								builtin_exit(t_execcmd *ecmd, t_vars *v);
char							*get_current_directory_string(char *buffer,
									size_t size);
int								builtin_env(char **envp);
char							**find_env_var(char **envp, const char *key);
int								builtin_export(t_execcmd *ecmd, char ***envp);
int								builtin_unset(t_execcmd *ecmd, char ***envp);

// BUILTIN EXPORT UTILS
int								is_valid_identifier(const char *str);
int								print_all_env(char **envp);
int								update_existing_var(char **var_ptr,
									char *arg_copy, char *value);
int								count_env_vars(char **envp);
int								copy_env_vars(char **new_envp, char **envp,
									int count);
int								add_new_var(char ***envp, char *arg_copy,
									char *value);
int								handle_var_assignment(char *key, char *value,
									char ***envp, char *arg_copy);
int								validate_and_process_key(char *key, char *value,
									char ***envp, char *arg_copy);
int								handle_no_value_case(char *key, char *arg_copy,
									char ***envp);
int								process_export_arg(char *arg, char ***envp);

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
void							perform_expansion(char **arg_ptr, char **envp);
char							*get_env_value(const char *key, char **envp);
void							extract_var_name(char **arg, char *var_name);
void							process_variable(char **arg, char *buffer,
									int *i, char **envp);
void							handle_exit_status_expansion(t_execcmd *ecmd,
									int i, int *p_last_exit_status);

// WILDCARDS '*'
typedef struct s_expand_vars
{
	char						*old_argv[MAX_ARGS];
	char						old_q[MAX_ARGS];
	int							old_alloc[MAX_ARGS];
	int							argc;
	int							i;
	int							j;
	int							t_count;
	char						*pat;
	char						*slash;
	int							dir_len;
	char						*dir_pat;
	char						*file_pat;
	DIR							*d1;
	struct dirent				*e1;
	struct stat					st;
	DIR							*d2;
	struct dirent				*e2;
	int							found;
	char						*matches[MAX_ARGS];
	int							match_count;
	char						*tmp_path;
	char						*tmp;
}								t_expand_vars;

int								expand_wildcards(t_execcmd *ecmd);
int								match_star(const char *pat, const char *name);
char							*expand_redirect_glob(char *pattern,
									int *allocated);
char							*ft_strndup(const char *s, size_t n);
void							sort_matches(char **matches, int count);
int								handle_dot_slash_pattern(char *pat,
									t_execcmd *ecmd, int *t_count);
int								handle_hidden_files(char *pat, t_execcmd *ecmd,
									int *t_count);
void							record_match(t_execcmd *ecmd, int *t_count,
									char *name);

void							init_expand_vars(t_execcmd *ecmd,
									t_expand_vars *vars);
int								handle_non_expandable_arg(t_execcmd *ecmd,
									t_expand_vars *vars);
int								handle_dot_slash_wildcard(t_execcmd *ecmd,
									t_expand_vars *vars);
int								handle_hidden_files_wildcard(t_execcmd *ecmd,
									t_expand_vars *vars);
void							add_directory_matches_to_argv(t_execcmd *ecmd,
									t_expand_vars *vars);
int								handle_slash_wildcard(t_execcmd *ecmd,
									t_expand_vars *vars);
void							process_simple_wildcard(t_expand_vars *vars);
void							add_simple_matches_to_argv(t_execcmd *ecmd,
									t_expand_vars *vars);
int								handle_simple_wildcard(t_execcmd *ecmd,
									t_expand_vars *vars);
void							handle_regular_arg(t_execcmd *ecmd,
									t_expand_vars *vars);
void							process_directory_matches(t_expand_vars *vars);

#endif
