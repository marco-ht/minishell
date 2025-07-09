#include "../includes/minishell.h"

// match '*'
static int match_star(const char *pat, const char *name)
{
    if (*pat == '\0')
        return (*name == '\0');
    if (*pat == '*')
    {
        while (*pat == '*') pat++;
        if (*pat == '\0') return 1;
        while (*name)
        {
            if (match_star(pat, name))
                return 1;
            name++;
        }
        return 0;
    }
    if (*pat == *name)
        return match_star(pat+1, name+1);
    return 0;
}

int expand_wildcards(t_execcmd *ecmd)
{
    char *old_argv[MAX_ARGS];
    char  old_q[MAX_ARGS];
    int   old_alloc[MAX_ARGS];
    int   argc;
    int i;
    int t_count;

    argc = 0
    t_count = 0;
    // salvo vecchi valori
    while (ecmd->argv[argc] && argc < MAX_ARGS)
        argc++;
    i = 0;
    while (i < argc)
    {
        old_argv[i]    = ecmd->argv[i];
        old_q[i]       = ecmd->qtype[i];
        old_alloc[i]   = ecmd->allocated[i];
        i++;
    }

    // costruisco t_argv, t_qtype, t_alloc
    i = 0;
    while (i < argc && t_count < MAX_ARGS-1)
    {
        char *pat = old_argv[i];

        if (ecmd->qtype[i] == 'a' && ft_strchr(pat, '*'))
        {
            DIR *d = opendir(".");
            struct dirent *e;
            int found = 0;

            if (d)
            {
                while ((e = readdir(d)))
                {
                    if (e->d_name[0] == '.')
                        continue;
                    if (match_star(pat, e->d_name))
                    {
                        // serve strdup e deve essere freeato allocated[t_count] = 1;
                        ecmd->argv[t_count]      = ft_strdup(e->d_name);
                        ecmd->qtype[t_count]     = 'a';
                        ecmd->allocated[t_count] = 1;
                        t_count++;
                        found = 1;
                        if (t_count >= MAX_ARGS-1)
                            break;
                    }
                }
                closedir(d);
            }
            if (!found && t_count < MAX_ARGS-1)
            {
                // nessuna corrispondenza: lascio il pattern literal
                ecmd->argv[t_count]      = old_argv[i];
                ecmd->qtype[t_count]     = old_q[i];
                ecmd->allocated[t_count] = old_alloc[i];
                t_count++;
            }
        }
        else
        {
            // non-atomico o senza '*'
            ecmd->argv[t_count]      = old_argv[i];
            ecmd->qtype[t_count]     = old_q[i];
            ecmd->allocated[t_count] = old_alloc[i];
            t_count++;
        }
        i++;
    }

    // 3) terminatore e zero-fill
    ecmd->argv[t_count]      = NULL;
    ecmd->qtype[t_count]     = '\0';
    ecmd->allocated[t_count] = 0;
    t_count++;
    while (t_count < MAX_ARGS)
    {
        ecmd->argv[t_count]      = NULL;
        ecmd->qtype[t_count]     = '\0';
        ecmd->allocated[t_count] = 0;
        t_count++;
    }

    return 0;
}
