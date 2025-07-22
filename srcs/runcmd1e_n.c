/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runcmd1e_n.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierant & sfelici <marvin@student.42ro    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 14:27:07 by mpierant &        #+#    #+#             */
/*   Updated: 2025/07/22 16:25:55 by mpierant &       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	handle_redir_setup(t_redir_ctx *ctx, t_cmd *cmd,
		int *p_last_exit_status)
{
	ctx->n = 0;
	ctx->cur = cmd;
	ctx->saved_stdin = dup(STDIN_FILENO);
	ctx->saved_stdout = dup(STDOUT_FILENO);
	if (ctx->saved_stdin < 0 || ctx->saved_stdout < 0)
	{
		perror("dup");
		update_exit_status(1, p_last_exit_status);
		return (1);
	}
	while (ctx->cur->type == REDIR)
	{
		ctx->redirs[ctx->n] = (t_redircmd *)ctx->cur;
		ctx->n++;
		ctx->cur = ctx->redirs[ctx->n - 1]->cmd;
	}
	return (0);
}

static int	handle_redir_open_and_error(t_redir_ctx *ctx,
		int *p_last_exit_status, t_redircmd *rcmd)
{
	int	fd;

	fd = open(rcmd->file, rcmd->mode, 0644);
	if (fd < 0)
	{
		ft_putstr_fd(rcmd->file, 2);
		if (errno == ENOENT)
			ft_putstr_fd(": No such file or directory\n", 2);
		else
			ft_putstr_fd(": Permission denied\n", 2);
		update_exit_status(1, p_last_exit_status);
		dup2(ctx->saved_stdin, STDIN_FILENO);
		close(ctx->saved_stdin);
		dup2(ctx->saved_stdout, STDOUT_FILENO);
		close(ctx->saved_stdout);
		return (-1);
	}
	return (fd);
}

static int	handle_redir_dup_and_error(t_redir_ctx *ctx,
		int *p_last_exit_status, t_redircmd *rcmd, int fd)
{
	if (dup2(fd, rcmd->fd) < 0)
	{
		perror("dup2");
		close(fd);
		update_exit_status(1, p_last_exit_status);
		dup2(ctx->saved_stdin, STDIN_FILENO);
		close(ctx->saved_stdin);
		dup2(ctx->saved_stdout, STDOUT_FILENO);
		close(ctx->saved_stdout);
		return (-1);
	}
	close(fd);
	return (0);
}

static int	handle_redir_apply(t_redir_ctx *ctx, int *p_last_exit_status)
{
	int			i;
	int			fd;
	t_redircmd	*rcmd;

	i = ctx->n - 1;
	while (i >= 0)
	{
		rcmd = ctx->redirs[i];
		fd = handle_redir_open_and_error(ctx, p_last_exit_status, rcmd);
		if (fd == -1)
			return (1);
		if (handle_redir_dup_and_error(ctx, p_last_exit_status, rcmd, fd) == -1)
			return (1);
		i--;
	}
	return (0);
}

int	run_redir_cmd(t_cmd *cmd, char ***envp, int *p_last_exit_status, t_vars *v)
{
	t_redir_ctx	ctx;

	if (handle_redir_setup(&ctx, cmd, p_last_exit_status) != 0)
		return (1);
	if (handle_redir_apply(&ctx, p_last_exit_status) != 0)
		return (1);
	ctx.status = ft_runcmd(ctx.cur, envp, p_last_exit_status, v);
	dup2(ctx.saved_stdin, STDIN_FILENO);
	close(ctx.saved_stdin);
	dup2(ctx.saved_stdout, STDOUT_FILENO);
	close(ctx.saved_stdout);
	return (ctx.status);
}
