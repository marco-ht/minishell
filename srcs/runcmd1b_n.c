/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runcmd1b.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierant & sfelici <marvin@student.42ro    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 13:34:09 by mpierant &        #+#    #+#             */
/*   Updated: 2025/07/21 13:44:55 by mpierant &       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	run_exec_exp_redirs_builtin(t_cmd *cmd, char ***envp,
		int *p_last_exit_status, t_execcmd **ecmd)
{
	int	status;

	*ecmd = (t_execcmd *)cmd;
	expand_wildcards(*ecmd);
	expand_variables(*ecmd, *envp, p_last_exit_status);
	remove_empty_args(*ecmd);
	if ((*ecmd)->argv[0] == NULL)
		return (update_exit_status(0, p_last_exit_status), 0);
	apply_redirs(cmd, p_last_exit_status);
	status = ft_check_builtin(*ecmd, envp, p_last_exit_status);
	if (status != -1)
		return (status);
	return (-2);
}

static void	run_exec_cmd_child(t_execcmd *ecmd, char **envp,
		int *p_last_exit_status, pid_t *pid)
{
	*pid = fork1();
	if (*pid == 0)
	{
		setup_signals_child();
		execve(find_path(ecmd->argv, envp), ecmd->argv, envp);
		if (errno == ENOENT)
		{
			update_exit_status(127, p_last_exit_status);
			free_tree((t_cmd *)ecmd);
			exit(127);
		}
		else
		{
			ft_putstr_fd(" Is a directory\n", 2);
			update_exit_status(126, p_last_exit_status);
			free_tree((t_cmd *)ecmd);
			exit(126);
		}
	}
}

int	run_exec_cmd(t_cmd *cmd, char ***envp, int *p_last_exit_status)
{
	int			status;
	pid_t		pid;
	t_execcmd	*ecmd;

	status = run_exec_exp_redirs_builtin(cmd, envp, p_last_exit_status, &ecmd);
	if (status != -2)
		return (status);
	run_exec_cmd_child(ecmd, *envp, p_last_exit_status, &pid);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (update_exit_status(WEXITSTATUS(status), p_last_exit_status),
			WEXITSTATUS(status));
	else
		return (update_exit_status(1, p_last_exit_status), 1);
}
