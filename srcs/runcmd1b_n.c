/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runcmd1b_n.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierant & sfelici <marvin@student.42ro    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 13:34:09 by mpierant &        #+#    #+#             */
/*   Updated: 2025/07/23 01:11:42 by mpierant &       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	run_exec_exp_redirs_builtin(t_cmd *cmd, int *p_last_exit_status,
		t_execcmd **ecmd, t_vars *v)
{
	int	status;

	*ecmd = (t_execcmd *)cmd;
	expand_wildcards(*ecmd);
	expand_variables(*ecmd, v->envp, p_last_exit_status);
	remove_empty_args(*ecmd);
	if ((*ecmd)->argv[0] == NULL)
		return (update_exit_status(0, p_last_exit_status), 0);
	apply_redirs(cmd, p_last_exit_status, v);
	status = ft_check_builtin(*ecmd, &v->envp, p_last_exit_status, v);
	if (status != -1)
		return (status);
	return (-2);
}

static void	run_exec_cmd_child(t_execcmd *ecmd, char **envp, pid_t *pid,
		t_vars *v)
{
	char	*path;
	int		exit_code;

	*pid = fork1();
	if (*pid == 0)
	{
		setup_signals_child();
		path = find_path(ecmd->argv, envp);
		if (path == NULL && errno == ENOENT)
			ft_exit_err_n(NULL, &v->last_exit_status, 127, v);
		if (path == NULL)
			ft_exit_err_n(NULL, &v->last_exit_status, 126, v);
		execve(path, ecmd->argv, envp);
		free(path);
		ft_putstr_fd(ecmd->argv[0], 2);
		ft_putstr_fd(": ", 2);
		perror("");
		free_tree(v->tree);
		ft_free_envp(envp);
		exit_code = 126;
		if (errno == ENOENT)
			exit_code = 127;
		update_exit_status(exit_code, &v->last_exit_status);
		exit(exit_code);
	}
}

int	run_exec_cmd(t_cmd *cmd, char ***envp, int *p_last_exit_status, t_vars *v)
{
	int			status;
	pid_t		pid;
	t_execcmd	*ecmd;

	status = run_exec_exp_redirs_builtin(cmd, p_last_exit_status, &ecmd, v);
	if (status != -2)
		return (status);
	run_exec_cmd_child(ecmd, *envp, &pid, v);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (update_exit_status(WEXITSTATUS(status), p_last_exit_status),
			WEXITSTATUS(status));
	else
		return (update_exit_status(1, p_last_exit_status), 1);
}
