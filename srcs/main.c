#include "../includes/minishell.h"

int main(void)
{
    char    *str;

    while(1)
    {
        str = readline("$ ");
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
