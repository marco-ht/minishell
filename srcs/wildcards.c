#include "../includes/minishell.h"

static void init_expand_vars(t_execcmd *ecmd, t_expand_vars *vars)
{
	vars->argc = 0;
	while (ecmd->argv[vars->argc] && vars->argc < MAX_ARGS)
		vars->argc++;
	vars->i = 0;
	while (vars->i < vars->argc)
	{
		vars->old_argv[vars->i] = ecmd->argv[vars->i];
		vars->old_q[vars->i] = ecmd->qtype[vars->i];
		vars->old_alloc[vars->i] = ecmd->allocated[vars->i];
		vars->i++;
	}
	vars->i = 0;
	vars->t_count = 0;
}

static int handle_non_expandable_arg(t_execcmd *ecmd, t_expand_vars *vars)
{
	if (vars->old_q[vars->i] != 'a')
	{
		ecmd->argv[vars->t_count] = vars->old_argv[vars->i];
		ecmd->qtype[vars->t_count] = vars->old_q[vars->i];
		ecmd->allocated[vars->t_count] = vars->old_alloc[vars->i];
		vars->t_count++;
		vars->i++;
		return 1;
	}
	return 0;
}

static int handle_dot_slash_wildcard(t_execcmd *ecmd, t_expand_vars *vars)
{
	if (strncmp(vars->pat, "./", 2) == 0 && ft_strchr(vars->pat, '*'))
	{
		vars->found = handle_dot_slash_pattern(vars->pat, ecmd, &vars->t_count);
		if (vars->found == 0)
		{
			ecmd->argv[vars->t_count] = vars->old_argv[vars->i];
			ecmd->qtype[vars->t_count] = vars->old_q[vars->i];
			ecmd->allocated[vars->t_count] = vars->old_alloc[vars->i];
			vars->t_count++;
		}
		vars->i++;
		return 1;
	}
	return 0;
}

static int handle_hidden_files_wildcard(t_execcmd *ecmd, t_expand_vars *vars)
{
	if (vars->pat[0] == '.' && ft_strchr(vars->pat, '*'))
	{
		vars->found = handle_hidden_files(vars->pat, ecmd, &vars->t_count);
		if (vars->found == 0)
		{
			ecmd->argv[vars->t_count] = vars->old_argv[vars->i];
			ecmd->qtype[vars->t_count] = vars->old_q[vars->i];
			ecmd->allocated[vars->t_count] = vars->old_alloc[vars->i];
			vars->t_count++;
		}
		vars->i++;
		return 1;
	}
	return 0;
}

static void process_directory_matches(t_expand_vars *vars)
{
	vars->d1 = opendir(".");
	if (vars->d1)
	{
		while ((vars->e1 = readdir(vars->d1)))
		{
			if (vars->e1->d_name[0] == '.')
				continue ;
			if (match_star(vars->dir_pat, vars->e1->d_name))
			{
				if (stat(vars->e1->d_name, &vars->st) == 0 && S_ISDIR(vars->st.st_mode))
				{
					vars->d2 = opendir(vars->e1->d_name);
					if (vars->d2)
					{
						while ((vars->e2 = readdir(vars->d2)))
						{
							if (vars->e2->d_name[0] == '.')
								continue ;
							if (match_star(vars->file_pat, vars->e2->d_name))
							{
								vars->tmp = ft_strjoin(vars->e1->d_name, "/");
								vars->tmp_path = ft_strjoin(vars->tmp, vars->e2->d_name);
								free(vars->tmp);
								vars->matches[vars->match_count] = vars->tmp_path;
								vars->match_count++;
								vars->found = 1;
							}
						}
						closedir(vars->d2);
					}
				}
			}
		}
		closedir(vars->d1);
	}
}

static void add_directory_matches_to_argv(t_execcmd *ecmd, t_expand_vars *vars)
{
	if (vars->match_count > 0)
	{
		sort_matches(vars->matches, vars->match_count);
		vars->i = 0;
		while (vars->i < vars->match_count && vars->t_count < MAX_ARGS - 1)
		{
			ecmd->argv[vars->t_count] = vars->matches[vars->i];
			ecmd->qtype[vars->t_count] = 'a';
			ecmd->allocated[vars->t_count] = 1;
			vars->t_count++;
			vars->i++;
		}
	}
}

static int handle_slash_wildcard(t_execcmd *ecmd, t_expand_vars *vars)
{
	vars->slash = ft_strchr(vars->pat, '/');
	if (vars->slash)
	{
		vars->found = 0;
		vars->match_count = 0;
		vars->dir_len = vars->slash - vars->pat;
		vars->dir_pat = ft_strndup(vars->pat, vars->dir_len);
		vars->file_pat = ft_strdup(vars->slash + 1);
		
		process_directory_matches(vars);
		add_directory_matches_to_argv(ecmd, vars);
		
		free(vars->dir_pat);
		free(vars->file_pat);
		if (vars->found == 0)
		{
			ecmd->argv[vars->t_count] = vars->old_argv[vars->i];
			ecmd->qtype[vars->t_count] = vars->old_q[vars->i];
			ecmd->allocated[vars->t_count] = vars->old_alloc[vars->i];
			vars->t_count++;
		}
		vars->i++;
		return 1;
	}
	return 0;
}

static void process_simple_wildcard_matches(t_expand_vars *vars)
{
	vars->d1 = opendir(".");
	if (vars->d1)
	{
		while ((vars->e1 = readdir(vars->d1)))
		{
			if (vars->e1->d_name[0] == '.')
				continue ;
			if (match_star(vars->pat, vars->e1->d_name))
			{
				vars->matches[vars->match_count] = ft_strdup(vars->e1->d_name);
				vars->match_count++;
				vars->found = 1;
			}
		}
		closedir(vars->d1);
	}
}

static void add_simple_matches_to_argv(t_execcmd *ecmd, t_expand_vars *vars)
{
	if (vars->match_count > 0)
	{
		sort_matches(vars->matches, vars->match_count);
		vars->j = 0;
		while (vars->j < vars->match_count && vars->t_count < MAX_ARGS - 1)
		{
			ecmd->argv[vars->t_count] = vars->matches[vars->j];
			ecmd->qtype[vars->t_count] = 'a';
			ecmd->allocated[vars->t_count] = 1;
			vars->t_count++;
			++vars->j;
		}
	}
}

static int handle_simple_wildcard(t_execcmd *ecmd, t_expand_vars *vars)
{
	if (ft_strchr(vars->pat, '*'))
	{
		vars->found = 0;
		vars->match_count = 0;
		
		process_simple_wildcard_matches(vars);
		add_simple_matches_to_argv(ecmd, vars);
		
		if (vars->found == 0)
		{
			ecmd->argv[vars->t_count] = vars->old_argv[vars->i];
			ecmd->qtype[vars->t_count] = vars->old_q[vars->i];
			ecmd->allocated[vars->t_count] = vars->old_alloc[vars->i];
			vars->t_count++;
		}
		vars->i++;
		return 1;
	}
	return 0;
}

static void handle_regular_arg(t_execcmd *ecmd, t_expand_vars *vars)
{
	ecmd->argv[vars->t_count] = vars->old_argv[vars->i];
	ecmd->qtype[vars->t_count] = vars->old_q[vars->i];
	ecmd->allocated[vars->t_count] = vars->old_alloc[vars->i];
	vars->t_count++;
	vars->i++;
}

static void finalize_argv(t_execcmd *ecmd, t_expand_vars *vars)
{
	ecmd->argv[vars->t_count] = NULL;
	ecmd->qtype[vars->t_count] = '\0';
	ecmd->allocated[vars->t_count] = 0;
	vars->t_count++;
	while (vars->t_count < MAX_ARGS)
	{
		ecmd->argv[vars->t_count] = NULL;
		ecmd->qtype[vars->t_count] = '\0';
		ecmd->allocated[vars->t_count] = 0;
		vars->t_count++;
	}
}

int	expand_wildcards(t_execcmd *ecmd)
{
	t_expand_vars	vars;

	init_expand_vars(ecmd, &vars);
	
	while (vars.i < vars.argc && vars.t_count < MAX_ARGS - 1)
	{
		vars.pat = vars.old_argv[vars.i];
		
		if (handle_non_expandable_arg(ecmd, &vars))
			continue ;
		if (handle_dot_slash_wildcard(ecmd, &vars))
			continue ;
		if (handle_hidden_files_wildcard(ecmd, &vars))
			continue ;
		if (handle_slash_wildcard(ecmd, &vars))
			continue ;
		if (handle_simple_wildcard(ecmd, &vars))
			continue ;
		
		handle_regular_arg(ecmd, &vars);
	}
	
	finalize_argv(ecmd, &vars);
	return (0);
}
