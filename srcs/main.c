#include "../includes/minishell.h"

int main(int argc, char **argv, char **env)
{
	char *buf;
	char *tmp;
	char *more;
	char *lineparser;
	int status;
	char **envp;
	t_cmd *tree;

	if (argc && argv[0])
		envp = ft_envcpy(env);
	setup_signals_interactive();
	while (ft_getcmd(&buf) >= 0)
	{
		reset_signal_flag();
		while (1)
		{
			lineparser = strdup(buf);
			tree = ft_parsecmd(lineparser, &status);
			if (tree == NULL && status == 1)
			{
				free(lineparser);
				more = readline("> ");
				if (!more)
					break;
				buf = ft_strjoin(ft_strjoin(buf, "\n"), more);
				free(more);
				continue;
			}
			break;
		}
		if (!more && tree == NULL)
		{
			free(lineparser);
			break;
		}
		tmp = buf;
		while (*tmp)
		{
			if (*tmp == '\n')
				*tmp = ' ';
			tmp++;
		}
		add_history(buf);
		ft_runcmd(tree, &envp);
		free(lineparser);
		free_tree(tree);
		free(buf);
	}
	return (free(buf), ft_free_envp(envp), rl_clear_history(), 0);
}

/*
valgrind --leak-check=full --show-leak-kinds=all --log-file=valgrind_output.txt ./nome_programma
*/
