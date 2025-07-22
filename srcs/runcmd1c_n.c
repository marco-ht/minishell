/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runcmd1c_n.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierant & sfelici <marvin@student.42ro    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 13:46:22 by mpierant &        #+#    #+#             */
/*   Updated: 2025/07/23 00:38:31 by mpierant &       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	run_heredoc_write(int p[2], t_heredoccmd *hcmd,
		int *status, t_vars *v)
{
	pid_t	pid_here;

	pid_here = fork1();
	if (pid_here == 0)
	{
		close(p[0]);
		ft_child(p, hcmd, v->envp);
		rl_clear_history();
		free(v->buf);
		free_tree(v->tree);
		ft_free_envp(v->envp);
		exit(0);
	}
	close(p[1]);
	waitpid(pid_here, status, 0);
}

int	run_heredoc_cmd(t_cmd *cmd, char ***envp, int *p_last_exit_status,
		t_vars *v)
{
	int				p[2];
	int				status;
	pid_t			pid;
	t_heredoccmd	*hcmd;

	hcmd = (t_heredoccmd *)cmd;
	if (pipe(p) < 0)
		ft_exit_err_n("pipe", p_last_exit_status, 1, v);
	run_heredoc_write(p, hcmd, &status, v);
	pid = fork1();
	if (pid == 0)
	{
		dup2(p[0], STDIN_FILENO);
		close(p[0]);
		status = ft_runcmd(hcmd->cmd, envp, p_last_exit_status, v);
		free_tree(v->tree);
		ft_free_envp(*envp);
		exit(status);
	}
	close(p[0]);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (update_exit_status(WEXITSTATUS(status), p_last_exit_status),
			WEXITSTATUS(status));
	return (update_exit_status(1, p_last_exit_status), 1);
}
