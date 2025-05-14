#include "../includes/minishell.h"

int main(void)
{
	char	*str;
	char	**matrix;
	t_exec	exec_str;

    while(1)
    {
		str = readline("$ ");
		if (!str || ft_strncmp(str, "exit", 4) == 0) //useremo la buildin exit, questo e' solo un esempio iniziale
			break;
		/* printf("str = %s\n", str); */
		set_starts(str, &exec_str);
		set_zeros(str, &exec_str);
		add_history(str);
		/* printf("str = %s\n", *(ft_split(str, ' ')));
		printf("str = %s\n", *((ft_split(str, ' '))+1)); */
		matrix = ft_split(str, ' ');
		if(fork() == 0)
			execvp(matrix[0], matrix);
		wait(NULL);
		/* printf("Ciao, %s\n", str); */
    }
		return(free(str), rl_clear_history(), 0);
}
/*
valgrind --leak-check=full --show-leak-kinds=all ./nome_programma
*/
