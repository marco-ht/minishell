#include "../includes/minishell.h"

t_cmd *ft_tree_pipe(void)
{
	t_execcmd *left;
	t_execcmd *right;
	t_pipecmd *pipe;

	left = malloc(sizeof(*left));
	memset(left, 0, sizeof(*left));
	left->type = EXEC;
	left->argv[0] = "ls";
	left->argv[1] = NULL;

	right = malloc(sizeof(*right));
	memset(right, 0, sizeof(*right));
	right->type = EXEC;
	right->argv[0] = "wc";
	right->argv[1] = NULL;

	pipe = malloc(sizeof(*pipe));
	memset(pipe, 0, sizeof(*pipe));
	pipe->type = PIPE;
	pipe->left = (t_cmd *)left;
	pipe->right = (t_cmd *)right;

	return ((t_cmd *)pipe);
}

t_cmd *ft_tree_pipe2(void)
{
	t_redircmd	*left;
	t_execcmd	*right;
	t_pipecmd	*pipe;
	t_execcmd	*ex;

	ex = malloc(sizeof(*ex));
	memset(ex, 0, sizeof(*ex));
	ex->type = EXEC;
	ex->argv[0] = "grep";
	ex->argv[1] = "ciao";
	ex->argv[2] = NULL;

	left = malloc(sizeof(*left));
	memset(left, 0, sizeof(*left));
	left->type = REDIR;
	left->cmd = (t_cmd *) ex;
	left->file = "test";
	left->efile = "";
	left->mode = O_APPEND;
	left->fd = 0;

	right = malloc(sizeof(*right));
	memset(right, 0, sizeof(*right));
	right->type = EXEC;
	right->argv[0] = "grep";
	right->argv[1] = "vediamo";
	right->argv[2] = NULL;

	pipe = malloc(sizeof(*pipe));
	memset(pipe, 0, sizeof(*pipe));
	pipe->type = PIPE;
	pipe->left = (t_cmd *)left;
	pipe->right = (t_cmd *)right;

	return ((t_cmd *)pipe);
}
t_cmd *ft_tree_pipe3(void)
{
	t_redircmd	*left;
	t_execcmd	*right;
	t_pipecmd	*pipe;
	t_execcmd	*ex;
	t_execcmd	*ex2;
	t_listcmd		*and;

	ex = malloc(sizeof(*ex));
	memset(ex, 0, sizeof(*ex));
	ex->type = EXEC;
	ex->argv[0] = "grep";
	ex->argv[1] = "ciao";
	ex->argv[2] = NULL;

	ex2 = malloc(sizeof(*ex2));
	memset(ex2, 0, sizeof(*ex2));
	ex2->type = EXEC;
	ex2->argv[0] = "echo";
	ex2->argv[1] = "quindi_funziona";
	ex2->argv[2] = NULL;

	left = malloc(sizeof(*left));
	memset(left, 0, sizeof(*left));
	left->type = REDIR;
	left->cmd = (t_cmd *) ex;
	left->file = "test";
	left->efile = "";
	left->mode = O_APPEND;
	left->fd = 0;

	right = malloc(sizeof(*right));
	memset(right, 0, sizeof(*right));
	right->type = EXEC;
	right->argv[0] = "grep";
	right->argv[1] = "vediamo";
	right->argv[2] = NULL;

	pipe = malloc(sizeof(*pipe));
	memset(pipe, 0, sizeof(*pipe));
	pipe->type = PIPE;
	pipe->left = (t_cmd *)left;
	pipe->right = (t_cmd *)right;

	and = malloc(sizeof(*and));
	memset(and, 0, sizeof(*and));
	and->type = LIST;
	and->left = (t_cmd *)pipe;
	and->right = (t_cmd *) ex2;

	return ((t_cmd *)and);
}
