#include "../includes/minishell.h"

int main(void)
{
	char *buf;
	t_cmd *tree;

	signal(SIGINT, handler);
	signal(SIGQUIT, SIG_IGN);

	//IMPORTANTE PER "ls |" CI VOGLIONO PIU ITERAZIONI
	//(vedi main modificato "Bash pipeline behavior")
	while (ft_getcmd(&buf) >= 0) //! buf (EOF, ctrl+D) -> -1 we stop while loop
	{
		add_history(buf);
		tree = ft_parsecmd(buf);
		ft_runcmd(tree);
		free_tree(tree);
		free(buf);
	}
	return (free(buf), rl_clear_history(), 0);
}

/*
valgrind --leak-check=full --show-leak-kinds=all --log-file=valgrind_output.txt ./nome_programma
*/
