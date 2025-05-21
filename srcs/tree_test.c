#include "../includes/minishell.h"

t_cmd	*ft_tree_test(void)
{
	t_execcmd	*left;
	t_execcmd	*right;
	t_pipecmd	*pipe;

	left = malloc(sizeof(*left));
	memset(left, 0, sizeof(*left));
	left->type = EXEC;
	left->argv[0] = "ls";
	left->argv[1] = "\0";

	right = malloc(sizeof(*right));
	memset(right, 0, sizeof(*right));
	right->type = EXEC;
	right->argv[0] = "wc";
	right->argv[1] = "\0";

	pipe = malloc(sizeof(*pipe));
	memset(pipe, 0, sizeof(*pipe));
	pipe->type = PIPE;
	pipe->left = (t_cmd *)left;
	pipe->right = (t_cmd *)right;

	return ((t_cmd *)pipe);
}
