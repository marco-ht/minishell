/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runcmd4_n.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierant & sfelici <marvin@student.42ro    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 00:47:12 by mpierant &        #+#    #+#             */
/*   Updated: 2025/07/21 02:28:02 by mpierant &       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	collect_redirs(t_cmd *cmd, t_redircmd **redirs)
{
	int		n;
	t_cmd	*cur;

	cur = cmd;
	n = 0;
	while (cur && cur->type == REDIR && n < 32)
	{
		redirs[n++] = (t_redircmd *)cur;
		cur = redirs[n - 1]->cmd;
	}
	return (n);
}

static void	process_did_redir(t_redircmd *rc, int *did)
{
	if (rc->fd == STDOUT_FILENO)
		*did = 1;
	else if (rc->fd == STDIN_FILENO)
		*did = 2;
}

static void	handle_open_and_dup(t_redircmd *rc, int *p_last_exit_status,
		int *did)
{
	int	fd;

	process_did_redir(rc, did);
	fd = open(rc->file, rc->mode, 0644);
	if (fd < 0)
	{
		ft_putstr_fd(rc->file, 2);
		if (errno == ENOENT)
			ft_putstr_fd(": No such file or directory\n", 2);
		else
			ft_putstr_fd(": Permission denied\n", 2);
		update_exit_status(1, p_last_exit_status);
		exit(1);
	}
	if (dup2(fd, rc->fd) < 0)
	{
		perror("dup2");
		close(fd);
		update_exit_status(1, p_last_exit_status);
		exit(1);
	}
	close(fd);
}

// returns 1 if fd[1] redirected
// 2 if fd[0] redirected
// 0 otherwise
int	apply_redirs(t_cmd *cmd, int *p_last_exit_status)
{
	t_redircmd	*redirs[32];
	int			n;
	int			i;
	int			did_redirect_std_in_out;

	did_redirect_std_in_out = 0;
	n = collect_redirs(cmd, redirs);
	i = n - 1;
	while (i >= 0)
	{
		handle_open_and_dup(redirs[i], p_last_exit_status,
			&did_redirect_std_in_out);
		i--;
	}
	return (did_redirect_std_in_out);
}
