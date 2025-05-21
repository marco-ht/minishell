#include "../includes/minishell.h"

int ft_getcmd(char *buf)
{
    buf = readline("$ ");
    if(!buf)
        return(-1); //EOF
    return(ft_strlen(buf));
}

void    ft_exit_err(char *str)
{
    printf("%s\n", str);
    exit(1);
}

int fork1(void)
{
    int pid;

    pid = fork();
    if(pid == -1)
        ft_exit_err("Fork");
    return(pid);
}
