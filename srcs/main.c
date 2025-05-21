#include "../includes/minishell.h"

int main(void)
{
    char	*buf;

    while(ft_getcmd(&buf) >= 0) //!buf (EOF, ctrl+D) -> -1 we stop while loop
	{
        add_history(buf);

		if (ft_strncmp(buf, "exit", 4) == 0) //useremo la buildin exit, questo e' solo un esempio iniziale
			break;

        /* ft_runcmd(ft_parsecmd(buf)); */
        /* if(fork1() == 0) */
        ft_runcmd(ft_tree_test());
        wait(NULL);
        free(buf);
	}
    return(free(buf), rl_clear_history(), 0);
}

/*
valgrind --leak-check=full --show-leak-kinds=all ./nome_programma
*/
