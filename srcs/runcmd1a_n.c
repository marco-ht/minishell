/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runcmd1a.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierant & sfelici <marvin@student.42ro    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 13:26:25 by mpierant &        #+#    #+#             */
/*   Updated: 2025/07/21 13:32:59 by mpierant &       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	run_execp_cmd(t_cmd *cmd, char ***envp, int *p_last_exit_status)
{
	int			status;
	t_execcmd	*ecmd;

	ecmd = (t_execcmd *)cmd;
	expand_wildcards(ecmd);
	expand_variables(ecmd, *envp, p_last_exit_status);
	remove_empty_args(ecmd);
	if (ecmd->argv[0] == NULL)
		return (update_exit_status(0, p_last_exit_status), 0);
	apply_redirs(cmd, p_last_exit_status);
	status = ft_check_builtin(ecmd, envp, p_last_exit_status);
	if (status != -1)
		return (status);
	execve(find_path(ecmd->argv, *envp), ecmd->argv, *envp);
	if (errno == ENOENT)
	{
		update_exit_status(127, p_last_exit_status);
		exit(127);
	}
	else
	{
		ft_putstr_fd(" Is a directory\n", 2);
		update_exit_status(126, p_last_exit_status);
		exit(126);
	}
}

int	run_and_cmd(t_cmd *cmd, char ***envp, int *p_last_exit_status)
{
	int			status_left;
	int			status_right;
	t_andcmd	*acmd;

	acmd = (t_andcmd *)cmd;
	status_left = ft_runcmd(acmd->left, envp, p_last_exit_status);
	if (status_left == 0)
	{
		status_right = ft_runcmd(acmd->right, envp, p_last_exit_status);
		return (update_exit_status(status_right, p_last_exit_status),
			status_right);
	}
	else
		return (update_exit_status(status_left, p_last_exit_status),
			status_left);
}

int	run_or_cmd(t_cmd *cmd, char ***envp, int *p_last_exit_status)
{
	int		status_left;
	int		status_right;
	t_orcmd	*ocmd;

	ocmd = (t_orcmd *)cmd;
	status_left = ft_runcmd(ocmd->left, envp, p_last_exit_status);
	if (status_left != 0)
	{
		status_right = ft_runcmd(ocmd->right, envp, p_last_exit_status);
		return (update_exit_status(status_right, p_last_exit_status),
			status_right);
	}
	else
		return (update_exit_status(0, p_last_exit_status), 0);
}

int	ft_runcmd(t_cmd *cmd, char ***envp, int *p_last_exit_status)
{
	if (cmd == NULL)
		return (0);
	if (cmd->type == EXEC)
		return (run_exec_cmd(cmd, envp, p_last_exit_status));
	else if (cmd->type == EXECP)
		return (run_execp_cmd(cmd, envp, p_last_exit_status));
	else if (cmd->type == REDIR)
		return (run_redir_cmd(cmd, envp, p_last_exit_status));
	else if (cmd->type == PIPE)
		return (run_pipe_cmd(cmd, envp, p_last_exit_status));
	else if (cmd->type == AND)
		return (run_and_cmd(cmd, envp, p_last_exit_status));
	else if (cmd->type == OR)
		return (run_or_cmd(cmd, envp, p_last_exit_status));
	else if (cmd->type == HEREDOC)
		return (run_heredoc_cmd(cmd, envp, p_last_exit_status));
	else
	{
		printf("runcmd error: unknown cmd type %d\n", cmd->type);
		return (1);
	}
	return (1);
}
