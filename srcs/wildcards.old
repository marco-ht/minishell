#include "../includes/minishell.h"

//ft_cpy_oldargv

int	expand_wildcards(t_execcmd *ecmd)
{
	t_expand_vars	vars;

	vars.argc = 0;
	while (ecmd->argv[vars.argc] && vars.argc < MAX_ARGS)
		vars.argc++;
	vars.i = 0;
	while (vars.i < vars.argc)
	{
		vars.old_argv[vars.i] = ecmd->argv[vars.i];
		vars.old_q[vars.i] = ecmd->qtype[vars.i];
		vars.old_alloc[vars.i] = ecmd->allocated[vars.i];
		vars.i++;
	}
	vars.i = 0;
	vars.t_count = 0;
	while (vars.i < vars.argc && vars.t_count < MAX_ARGS - 1)
	{
		vars.pat = vars.old_argv[vars.i];
		if (vars.old_q[vars.i] != 'a')
		{
			ecmd->argv[vars.t_count] = vars.old_argv[vars.i];
			ecmd->qtype[vars.t_count] = vars.old_q[vars.i];
			ecmd->allocated[vars.t_count] = vars.old_alloc[vars.i];
			vars.t_count++;
			vars.i++;
			continue ;
		}
		if (strncmp(vars.pat, "./", 2) == 0 && ft_strchr(vars.pat, '*'))
		{
			vars.found = handle_dot_slash_pattern(vars.pat, ecmd, &vars.t_count);
			if (vars.found == 0)
			{
				ecmd->argv[vars.t_count] = vars.old_argv[vars.i];
				ecmd->qtype[vars.t_count] = vars.old_q[vars.i];
				ecmd->allocated[vars.t_count] = vars.old_alloc[vars.i];
				vars.t_count++;
			}
			vars.i++;
			continue ;
		}
		if (vars.pat[0] == '.' && ft_strchr(vars.pat, '*'))
		{
			vars.found = handle_hidden_files(vars.pat, ecmd, &vars.t_count);
			if (vars.found == 0)
			{
				ecmd->argv[vars.t_count] = vars.old_argv[vars.i];
				ecmd->qtype[vars.t_count] = vars.old_q[vars.i];
				ecmd->allocated[vars.t_count] = vars.old_alloc[vars.i];
				vars.t_count++;
			}
			vars.i++;
			continue ;
		}
		vars.slash = ft_strchr(vars.pat, '/');
		if (vars.slash)
		{
			vars.found = 0;
			vars.match_count = 0;
			vars.dir_len = vars.slash - vars.pat;
			vars.dir_pat = ft_strndup(vars.pat, vars.dir_len);
			vars.file_pat = ft_strdup(vars.slash + 1);
			vars.d1 = opendir(".");
			if (vars.d1)
			{
				while ((vars.e1 = readdir(vars.d1)))
				{
					if (vars.e1->d_name[0] == '.')
						continue ;
					if (match_star(vars.dir_pat, vars.e1->d_name))
					{
						if (stat(vars.e1->d_name, &vars.st) == 0 && S_ISDIR(vars.st.st_mode))
						{
							vars.d2 = opendir(vars.e1->d_name);
							if (vars.d2)
							{
								while ((vars.e2 = readdir(vars.d2)))
								{
									if (vars.e2->d_name[0] == '.')
										continue ;
									if (match_star(vars.file_pat, vars.e2->d_name))
									{
										vars.tmp = ft_strjoin(vars.e1->d_name, "/");
										vars.tmp_path = ft_strjoin(vars.tmp, vars.e2->d_name);
										free(vars.tmp);
										vars.matches[vars.match_count] = vars.tmp_path;
										vars.match_count++;
										vars.found = 1;
									}
								}
								closedir(vars.d2);
							}
						}
					}
				}
				closedir(vars.d1);
			}
			if (vars.match_count > 0)
			{
				sort_matches(vars.matches, vars.match_count);
				vars.i = 0;
				while (vars.i < vars.match_count && vars.t_count < MAX_ARGS - 1)
				{
					ecmd->argv[vars.t_count] = vars.matches[vars.i];
					ecmd->qtype[vars.t_count] = 'a';
					ecmd->allocated[vars.t_count] = 1;
					vars.t_count++;
					vars.i++;
				}
			}
			free(vars.dir_pat);
			free(vars.file_pat);
			if (vars.found == 0)
			{
				ecmd->argv[vars.t_count] = vars.old_argv[vars.i];
				ecmd->qtype[vars.t_count] = vars.old_q[vars.i];
				ecmd->allocated[vars.t_count] = vars.old_alloc[vars.i];
				vars.t_count++;
			}
			vars.i++;
			continue ;
		}
		if (ft_strchr(vars.pat, '*'))
		{
			vars.found = 0;
			vars.match_count = 0;
			vars.d1 = opendir(".");
			if (vars.d1)
			{
				while ((vars.e1 = readdir(vars.d1)))
				{
					if (vars.e1->d_name[0] == '.')
						continue ;
					if (match_star(vars.pat, vars.e1->d_name))
					{
						vars.matches[vars.match_count] = ft_strdup(vars.e1->d_name);
						vars.match_count++;
						vars.found = 1;
					}
				}
				closedir(vars.d1);
			}
			if (vars.match_count > 0)
			{
				sort_matches(vars.matches, vars.match_count);
				vars.j = 0;
				while (vars.j < vars.match_count && vars.t_count < MAX_ARGS - 1)
				{
					ecmd->argv[vars.t_count] = vars.matches[vars.j];
					ecmd->qtype[vars.t_count] = 'a';
					ecmd->allocated[vars.t_count] = 1;
					vars.t_count++;
					++vars.j;
				}
			}
			if (vars.found == 0)
			{
				ecmd->argv[vars.t_count] = vars.old_argv[vars.i];
				ecmd->qtype[vars.t_count] = vars.old_q[vars.i];
				ecmd->allocated[vars.t_count] = vars.old_alloc[vars.i];
				vars.t_count++;
			}
			vars.i++;
			continue ;
		}
		ecmd->argv[vars.t_count] = vars.old_argv[vars.i];
		ecmd->qtype[vars.t_count] = vars.old_q[vars.i];
		ecmd->allocated[vars.t_count] = vars.old_alloc[vars.i];
		vars.t_count++;
		vars.i++;
	}
	ecmd->argv[vars.t_count] = NULL;
	ecmd->qtype[vars.t_count] = '\0';
	ecmd->allocated[vars.t_count] = 0;
	vars.t_count++;
	while (vars.t_count < MAX_ARGS)
	{
		ecmd->argv[vars.t_count] = NULL;
		ecmd->qtype[vars.t_count] = '\0';
		ecmd->allocated[vars.t_count] = 0;
		vars.t_count++;
	}
	return (0);
}
