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
	left->cmd = ex;
	left->file = "test";
	left->efile = "";
	left->mode = O_APPEND;
	left->fd = 0;

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
