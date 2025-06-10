#include "../includes/minishell.h"

void    ft_free_envp(char **envp)
{
    int i;

    if (!envp)
        return;
    i = -1;
    while (envp[++i])
        free(envp[i]);
    free(envp);
}

char    **ft_envcpy(char **env)
{
    int     i;
    int     j;
    char    **envp;

    i = 0;
    while (env[i])
        i++;
    envp = malloc((i + 1) * sizeof(char *));
    if (!envp)
    {
        perror("envcpy");
        exit (1);
    }
    i = -1;
    while (env[++i])
    {
        envp[i] = ft_strdup(env[i]);
        if (!envp[i])
        {
            j = -1;
            while (j < i)
                free(envp[++j]);
            free(envp);
            return NULL;
        }
    }
    envp[i] = NULL;
    return(envp);
}
