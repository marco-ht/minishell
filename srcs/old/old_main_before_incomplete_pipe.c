#include "../includes/minishell.h"

int main(int argc, char **argv, char **env)
{
	char *buf;
	char **envp;
	t_cmd *tree;

	if(argc && argv[0])
		envp = ft_envcpy(env);
	signal(SIGINT, handler);
	signal(SIGQUIT, SIG_IGN);
	//IMPORTANTE PER "ls |" CI VOGLIONO PIU ITERAZIONI
	//(vedi main modificato "Bash pipeline behavior")
	while (ft_getcmd(&buf) >= 0) // until EOF (Ctrl-D)
	{
		add_history(buf);
		tree = ft_parsecmd(buf);
		ft_runcmd(tree, envp);
		free_tree(tree);
		free(buf);
	}
	return (free(buf), ft_free_envp(envp), rl_clear_history(), 0);
}

/*
valgrind --leak-check=full --show-leak-kinds=all --log-file=valgrind_output.txt ./nome_programma
*/
