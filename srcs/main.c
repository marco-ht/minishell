#include "../includes/minishell.h"

int main(void)
{
	char *buf;
	t_cmd	*tree;

	while (ft_getcmd(&buf) >= 0) //! buf (EOF, ctrl+D) -> -1 we stop while loop
	{
		add_history(buf);

		if (ft_strncmp(buf, "exit", 4) == 0) // useremo la buildin exit, questo e' solo un esempio iniziale
			break;

		/* printf("buffer = %s\n", buf); */
		/* printf("pid main = %d\n", getpid()); */
		tree = ft_tree_pipe3();
		/* tree = ft_parsecmd(buf); */
		/* if (fork1() == 0) */
			ft_runcmd(tree);
		wait(NULL);
		free_tree(tree);
		free(buf);
	}
	return (free(buf), rl_clear_history(), 0);
}

/*
valgrind --leak-check=full --show-leak-kinds=all --log-file=valgrind_output.txt ./nome_programma
*/
