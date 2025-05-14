#include "../includes/minishell.h"

int main(void)
{
    char	*str;
    t_exec	exec_str;

    while(1)
    {
        str = readline("$ ");
        set_zeros(str, &exec_str);
		set_starts(str, &exec_str);
        if (ft_strncmp(str, "exit", 4) == 0)    //useremo la buildin exit (che prende anche un parametro facoltativo), questo e' solo un esempio iniziale
            return(free(str), rl_clear_history(), 0);
        if (*str)
            add_history(str);
        printf("Ciao, %s\n", str);
        free(str);
    }
}
/*
valgrind --leak-check=full --show-leak-kinds=all ./nome_programma
*/
