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

char *ft_strndup(const char *s, size_t n)
{
    char    *dup;
    size_t   len;
    size_t   i;

    len = 0;
    while (s[len] && len < n)
        len++;
    dup = malloc(len + 1);
    if (!dup)
        return NULL;
    i = 0;
    while (i < len)
    {
        dup[i] = s[i];
        i++;
    }
    dup[i] = '\0';
    return dup;
}

int expand_wildcards(t_execcmd *ecmd)
{
    char        *old_argv[MAX_ARGS];
    char         old_q[MAX_ARGS];
    int          old_alloc[MAX_ARGS];
    int          argc;
    int          i;
    int          t_count;
    char        *pat;
    char        *slash;
    int          dir_len;
    char        *dir_pat;
    char        *file_pat;
    DIR         *d1;
    struct dirent *e1;
    struct stat  st;
    DIR         *d2;
    struct dirent *e2;
    int          found;

    /* 1) salva lo stato originale */
    argc = 0;
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

    /* 2) ricostruisci in-place */
    i       = 0;
    t_count = 0;
    while (i < argc && t_count < MAX_ARGS - 1)
    {
        pat   = old_argv[i];
        slash = ft_strchr(pat, '/');

                /* pattern dir/file */
        if (old_q[i] == 'a' && slash)
        {
            /* 1) split sul primo slash */
            dir_len  = slash - pat;
            dir_pat  = ft_strndup(pat, dir_len);
            file_pat = ft_strdup(slash + 1);

            found = 0;
            d1 = opendir(".");
            if (d1)
            {
                while ((e1 = readdir(d1)) && t_count < MAX_ARGS - 1)
                {
                    /* salta entry nascoste */
                    if (e1->d_name[0] == '.')
                        continue;

                    /* match sulla parte di directory */
                    if (match_star(dir_pat, e1->d_name))
                    {
                        /* è davvero una directory? */
                        if (stat(e1->d_name, &st) == 0 && S_ISDIR(st.st_mode))
                        {
                            /* apro la sottodirectory */
                            d2 = opendir(e1->d_name);
                            if (d2)
                            {
                                while ((e2 = readdir(d2)) && t_count < MAX_ARGS - 1)
                                {
                                    /* skip “.” e “..” */
                                    if (e2->d_name[0] == '.')
                                        continue;

                                    /* match sulla parte di file */
                                    if (match_star(file_pat, e2->d_name))
                                    {
                                        char *tmp = ft_strjoin(e1->d_name, "/");
                                        ecmd->argv[t_count]      = ft_strjoin(tmp, e2->d_name);
                                        free(tmp);
                                        ecmd->qtype[t_count]     = 'a';
                                        ecmd->allocated[t_count] = 1;
                                        t_count++;
                                        found = 1;
                                    }
                                }
                                closedir(d2);
                            }
                        }
                    }
                }
                closedir(d1);
            }

            /* pulisco i buffer temporanei */
            free(dir_pat);
            free(file_pat);

            /* 4) fallback: se non ho aggiunto alcun “dir/file” */
            if (found == 0)
            {
                ecmd->argv[t_count]      = old_argv[i];
                ecmd->qtype[t_count]     = old_q[i];
                ecmd->allocated[t_count] = old_alloc[i];
                t_count++;
            }

            i++;
            continue;
        }


        /* pattern semplice * */
        if (old_q[i] == 'a' && ft_strchr(pat, '*'))
        {
            found = 0;
            d1    = opendir(".");
            if (d1)
            {
                while ((e1 = readdir(d1)) && t_count < MAX_ARGS - 1)
                {
                    if (e1->d_name[0] == '.')
                        continue;
                    if (match_star(pat, e1->d_name))
                    {
                        ecmd->argv[t_count]      = ft_strdup(e1->d_name);
                        ecmd->qtype[t_count]     = 'a';
                        ecmd->allocated[t_count] = 1;
                        t_count++;
                        found = 1;
                    }
                }
                closedir(d1);
            }
            if (found == 0)
            {
                ecmd->argv[t_count]      = old_argv[i];
                ecmd->qtype[t_count]     = old_q[i];
                ecmd->allocated[t_count] = old_alloc[i];
                t_count++;
            }
            i++;
            continue;
        }

        /* nessuna wildcard */
        ecmd->argv[t_count]      = old_argv[i];
        ecmd->qtype[t_count]     = old_q[i];
        ecmd->allocated[t_count] = old_alloc[i];
        t_count++;
        i++;
    }

    /* 3) terminatore e pulizia */
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
