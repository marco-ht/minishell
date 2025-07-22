/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runcmd1d_n.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierant & sfelici <marvin@student.42ro    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 14:02:43 by mpierant &        #+#    #+#             */
/*   Updated: 2025/07/22 05:50:23 by mpierant &       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	run_pipe_left(t_pipe_ctx *ctx, char ***envp,
		int *p_last_exit_status)
{
	int	redir_stdout;
	int	exit_status;

	ctx->pid_left = fork1();
	if (ctx->pid_left == 0)
	{
		setup_signals_child();
		redir_stdout = apply_redirs(ctx->pcmd->left, p_last_exit_status);
		if (redir_stdout != 1)
			dup2(ctx->p[1], 1);
		close(ctx->p[0]);
		close(ctx->p[1]);
		if (ctx->pcmd->left->type == EXEC)
			ctx->pcmd->left->type = EXECP;
		exit_status = ft_runcmd(ctx->pcmd->left, envp, p_last_exit_status);
		free_tree((t_cmd *)ctx->pcmd);
		ft_free_envp(*envp);
		exit(exit_status);
	}
}

static void	run_pipe_right(t_pipe_ctx *ctx, char ***envp,
		int *p_last_exit_status)
{
	int	redir_stdin;
	int	exit_status;

	ctx->pid_right = fork1();
	if (ctx->pid_right == 0)
	{
		setup_signals_child();
		redir_stdin = apply_redirs(ctx->pcmd->right, p_last_exit_status);
		if (redir_stdin != 2)
			dup2(ctx->p[0], 0);
		close(ctx->p[0]);
		close(ctx->p[1]);
		if (ctx->pcmd->right->type == EXEC)
			ctx->pcmd->right->type = EXECP;
		exit_status = ft_runcmd(ctx->pcmd->right, envp, p_last_exit_status);
		free_tree((t_cmd *)ctx->pcmd);
		ft_free_envp(*envp);
		exit(exit_status);
	}
}

int	run_pipe_cmd(t_cmd *cmd, char ***envp, int *p_last_exit_status)
{
	t_pipe_ctx	ctx;

	ctx.pcmd = (t_pipecmd *)cmd;
	if (pipe(ctx.p) < 0)
		ft_exit_err_n("pipe", p_last_exit_status);
	run_pipe_left(&ctx, envp, p_last_exit_status);
	run_pipe_right(&ctx, envp, p_last_exit_status);
	close(ctx.p[0]);
	close(ctx.p[1]);
	waitpid(ctx.pid_left, NULL, 0);
	waitpid(ctx.pid_right, &ctx.status_right, 0);
	if (WIFEXITED(ctx.status_right))
		return (update_exit_status(WEXITSTATUS(ctx.status_right),
				p_last_exit_status), WEXITSTATUS(ctx.status_right));
	else
		return (update_exit_status(1, p_last_exit_status), 1);
}
