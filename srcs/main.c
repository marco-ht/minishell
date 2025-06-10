#include "../includes/minishell.h"

int main(int argc, char **argv, char **env)
{
	char *buf;
	char **envp;
	t_cmd *tree;

	envp = ft_envcpy(env);
	if(!envp || !argc || !argv[0])
		return(perror("envcpy"), 1);
	signal(SIGINT, handler);
	signal(SIGQUIT, SIG_IGN);
	//IMPORTANTE PER "ls |" CI VOGLIONO PIU ITERAZIONI
	//(vedi main modificato "Bash pipeline behavior")
	while (ft_getcmd(&buf) >= 0) //ctrl+D == EOF -> -1 -> stop while loop
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
