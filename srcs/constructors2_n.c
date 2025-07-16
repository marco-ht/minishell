/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   constructors2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierant & sfelici <marvin@student.42ro    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 19:58:08 by mpierant &        #+#    #+#             */
/*   Updated: 2025/07/16 19:58:27 by mpierant &       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_cmd	*ft_heredoccmd(t_cmd *subcmd, int tok_type, char *lim_start,
		char *lim_end)
{
	t_heredoccmd	*hd;

	hd = malloc(sizeof(*hd));
	if (!hd)
		ft_exit_err("malloc");
	ft_memset(hd, 0, sizeof(*hd));
	hd->expand = 0;
	if (tok_type == 'a')
		hd->expand = 1;
	hd->type = HEREDOC;
	hd->cmd = subcmd;
	hd->lim_start = lim_start;
	hd->lim_end = lim_end;
	return ((t_cmd *)hd);
}
