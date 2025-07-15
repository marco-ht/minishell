#include "../includes/minishell.h"

// match '*'
static int match_star(const char *pat, const char *name)
{
    if (*pat == '\0')
        return (*name == '\0');
    if (*pat == '*')
    {
        while (*pat == '*')
            pat++;
        if (*pat == '\0')
            return 1;
        while (*name)
        {
            if (match_star(pat, name))
                return 1;
            name++;
        }
        return 0;
    }
    if (*pat == *name)
        return match_star(pat + 1, name + 1);
    return 0;
}

char *ft_strndup(const char *s, size_t n)
{
    char *dup;
    size_t len;
    size_t i;

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

// supporto per pattern che iniziano con "./"
static int handle_dot_slash_pattern(char *pat, t_execcmd *ecmd, int *t_count)
{
    DIR *d;
    struct dirent *e;
    char *match_pat;
    int found;

    found = 0;
    // Rimuovi "./" dall'inizio del pattern
    if (strncmp(pat, "./", 2) == 0)
        match_pat = pat + 2;
    else
        match_pat = pat;

    d = opendir(".");
    if (d)
    {
        while ((e = readdir(d)) && *t_count < MAX_ARGS - 1)
        {
            if (e->d_name[0] == '.' && match_pat[0] != '.')
                continue;
            if (match_star(match_pat, e->d_name))
            {
                char *full_path;

                full_path = malloc(strlen(e->d_name) + 3);
                strcpy(full_path, "./");
                strcat(full_path, e->d_name);
                ecmd->argv[*t_count] = full_path;
                ecmd->qtype[*t_count] = 'a';
                ecmd->allocated[*t_count] = 1;
                (*t_count)++;
                found = 1;
            }
        }
        closedir(d);
    }
    return found;
}

// supporto per file nascosti
static int handle_hidden_files(char *pat, t_execcmd *ecmd, int *t_count)
{
    DIR *d;
    struct dirent *e;
    int found;

    found = 0;
    if (pat[0] == '.' && pat[1] == '*')
    {
        d = opendir(".");
        if (d)
        {
            while ((e = readdir(d)) && *t_count < MAX_ARGS - 1)
            {
                if (e->d_name[0] == '.' && 
                    strcmp(e->d_name, ".") != 0 && 
                    strcmp(e->d_name, "..") != 0)
                {
                    if (match_star(pat, e->d_name))
                    {
                        ecmd->argv[*t_count] = ft_strdup(e->d_name);
                        ecmd->qtype[*t_count] = 'a';
                        ecmd->allocated[*t_count] = 1;
                        (*t_count)++;
                        found = 1;
                    }
                }
            }
            closedir(d);
        }
    }
    return found;
}

// bubble sort per ordinamento alfabetico
static void sort_matches(char **matches, int count)
{
    int i;
    int j;
    char *tmp;

    i = 0;
    while (i < count - 1)
    {
        j = 0;
        while (j < count - i - 1)
        {
            if (strcmp(matches[j], matches[j + 1]) > 0)
            {
                tmp = matches[j];
                matches[j] = matches[j + 1];
                matches[j + 1] = tmp;
            }
            j++;
        }
        i++;
    }
}

int expand_wildcards(t_execcmd *ecmd)
{
    char *old_argv[MAX_ARGS];
    char old_q[MAX_ARGS];
    int old_alloc[MAX_ARGS];
    int argc;
    int i;
    int j;
    int t_count;
    char *pat;
    char *slash;
    int dir_len;
    char *dir_pat;
    char *file_pat;
    DIR *d1;
    struct dirent *e1;
    struct stat st;
    DIR *d2;
    struct dirent *e2;
    int found;
    char *matches[MAX_ARGS];
    int match_count;

    argc = 0;
    while (ecmd->argv[argc] && argc < MAX_ARGS)
        argc++;
    i = 0;
    while (i < argc)
    {
        old_argv[i] = ecmd->argv[i];
        old_q[i] = ecmd->qtype[i];
        old_alloc[i] = ecmd->allocated[i];
        i++;
    }
    i = 0;
    t_count = 0;
    while (i < argc && t_count < MAX_ARGS - 1)
    {
        pat = old_argv[i];
        if (old_q[i] != 'a')
        {
            ecmd->argv[t_count] = old_argv[i];
            ecmd->qtype[t_count] = old_q[i];
            ecmd->allocated[t_count] = old_alloc[i];
            t_count++;
            i++;
            continue;
        }

        if (strncmp(pat, "./", 2) == 0 && ft_strchr(pat, '*'))
        {
            found = handle_dot_slash_pattern(pat, ecmd, &t_count);
            if (found == 0)
            {
                ecmd->argv[t_count] = old_argv[i];
                ecmd->qtype[t_count] = old_q[i];
                ecmd->allocated[t_count] = old_alloc[i];
                t_count++;
            }
            i++;
            continue;
        }
        if (pat[0] == '.' && ft_strchr(pat, '*'))
        {
            found = handle_hidden_files(pat, ecmd, &t_count);
            if (found == 0)
            {
                ecmd->argv[t_count] = old_argv[i];
                ecmd->qtype[t_count] = old_q[i];
                ecmd->allocated[t_count] = old_alloc[i];
                t_count++;
            }
            i++;
            continue;
        }
        slash = ft_strchr(pat, '/');
        if (slash)
        {
            found = 0;
            match_count = 0;
            dir_len = slash - pat;
            dir_pat = ft_strndup(pat, dir_len);
            file_pat = ft_strdup(slash + 1);
            d1 = opendir(".");
            if (d1)
            {
                while ((e1 = readdir(d1)))
                {
                    if (e1->d_name[0] == '.')
                        continue;
                    if (match_star(dir_pat, e1->d_name))
                    {
                        if (stat(e1->d_name, &st) == 0 && S_ISDIR(st.st_mode))
                        {
                            d2 = opendir(e1->d_name);
                            if (d2)
                            {
                                while ((e2 = readdir(d2)))
                                {
                                    if (e2->d_name[0] == '.')
                                        continue;
                                    if (match_star(file_pat, e2->d_name))
                                    {
                                        char *tmp_path;
                                        char *tmp;

                                        tmp = ft_strjoin(e1->d_name, "/");
                                        tmp_path = ft_strjoin(tmp, e2->d_name);
                                        free(tmp);
                                        matches[match_count] = tmp_path;
                                        match_count++;
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
            if (match_count > 0)
            {
                sort_matches(matches, match_count);
                i = 0;
                while (i < match_count && t_count < MAX_ARGS - 1)
                {
                    ecmd->argv[t_count] = matches[i];
                    ecmd->qtype[t_count] = 'a';
                    ecmd->allocated[t_count] = 1;
                    t_count++;
                    i++;
                }
            }
            free(dir_pat);
            free(file_pat);
            if (found == 0)
            {
                ecmd->argv[t_count] = old_argv[i];
                ecmd->qtype[t_count] = old_q[i];
                ecmd->allocated[t_count] = old_alloc[i];
                t_count++;
            }
            i++;
            continue;
        }
        if (ft_strchr(pat, '*'))
        {
            found = 0;
            match_count = 0;
            d1 = opendir(".");
            if (d1)
            {
                while ((e1 = readdir(d1)))
                {
                    if (e1->d_name[0] == '.')
                        continue;
                    if (match_star(pat, e1->d_name))
                    {
                        matches[match_count] = ft_strdup(e1->d_name);
                        match_count++;
                        found = 1;
                    }
                }
                closedir(d1);
            }
            if (match_count > 0)
            {
                sort_matches(matches, match_count);
                j = 0;
                while (j < match_count && t_count < MAX_ARGS - 1)
                {
                    ecmd->argv[t_count] = matches[j];
                    ecmd->qtype[t_count] = 'a';
                    ecmd->allocated[t_count] = 1;
                    t_count++;
                    ++j;
                }
            }
            if (found == 0)
            {
                ecmd->argv[t_count] = old_argv[i];
                ecmd->qtype[t_count] = old_q[i];
                ecmd->allocated[t_count] = old_alloc[i];
                t_count++;
            }
            i++;
            continue;
        }
        ecmd->argv[t_count] = old_argv[i];
        ecmd->qtype[t_count] = old_q[i];
        ecmd->allocated[t_count] = old_alloc[i];
        t_count++;
        i++;
    }
    ecmd->argv[t_count] = NULL;
    ecmd->qtype[t_count] = '\0';
    ecmd->allocated[t_count] = 0;
    t_count++;
    while (t_count < MAX_ARGS)
    {
        ecmd->argv[t_count] = NULL;
        ecmd->qtype[t_count] = '\0';
        ecmd->allocated[t_count] = 0;
        t_count++;
    }
    return 0;
}
