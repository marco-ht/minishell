#include "../includes/minishell.h"

void	builtin_pwd(t_execcmd *ecmd)
{
	char cwd_buffer[1024];

	(void)ecmd;
	if (getcwd(cwd_buffer, sizeof(cwd_buffer)) != NULL) {
		printf("%s\n", cwd_buffer);
	} else {
		perror("minishell: pwd");
	}
}
