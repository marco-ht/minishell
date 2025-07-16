/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_tree2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierant & sfelici <marvin@student.42ro    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 14:10:04 by mpierant &        #+#    #+#             */
/*   Updated: 2025/07/16 14:10:05 by mpierant &       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_exec(t_execcmd *ex)
{
	int	i;

	i = -1;
	while (ex->argv[++i])
	{
		if (ex->allocated[i])
			free(ex->argv[i]);
	}
	free(ex);
}

void	free_pipe_cmd(t_pipecmd *pipecmd)
{
	free_tree(pipecmd->left);
	free_tree(pipecmd->right);
	free(pipecmd);
}
