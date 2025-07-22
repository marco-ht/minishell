/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_tree2_n.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierant & sfelici <marvin@student.42ro    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 14:10:04 by mpierant &        #+#    #+#             */
/*   Updated: 2025/07/22 02:22:46 by mpierant &       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_exec(t_execcmd *ex)
{
	int	i;

	i = 0;
	while (i < MAX_ARGS)
	{
		if (ex->allocated[i])
			free(ex->argv[i]);
		i++;
	}
	free(ex);
}

void	free_pipe_cmd(t_pipecmd *pipecmd)
{
	free_tree(pipecmd->left);
	free_tree(pipecmd->right);
	free(pipecmd);
}
