/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runcmd3_n.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierant & sfelici <marvin@student.42ro    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 00:24:32 by mpierant &        #+#    #+#             */
/*   Updated: 2025/07/21 21:22:29 by mpierant &       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	gnl_cleanup(void)
{
	static char	*str;

	if (str)
	{
		free(str);
		str = NULL;
	}
}

/* function to remove argv[i]=="" empty strings */
void	remove_empty_args(t_execcmd *ecmd)
{
	int	read_i;
	int	write_i;

	read_i = 0;
	write_i = 0;
	while (ecmd->argv[read_i])
	{
		if (ecmd->argv[read_i][0] != '\0')
		{
			ecmd->argv[write_i] = ecmd->argv[read_i];
			ecmd->eargv[write_i] = ecmd->eargv[read_i];
			ecmd->qtype[write_i] = ecmd->qtype[read_i];
			ecmd->allocated[write_i] = ecmd->allocated[read_i];
			write_i++;
		}
		read_i++;
	}
	ecmd->argv[write_i] = NULL;
	ecmd->eargv[write_i] = NULL;
}

void	ft_child(int *fd, t_heredoccmd *hcmd, char **envp)
{
	char	*line;

	close(fd[0]);
	line = get_next_line(STDIN_FILENO);
	while (line != NULL)
	{
		if (ft_strncmp(line, hcmd->lim_start, ft_strlen(hcmd->lim_start)) == 0)
		{
			free(line);
			get_next_line(-1);
			close(fd[1]);
			return ;
		}
		if (hcmd->expand && ft_strchr(line, '$'))
			perform_expansion(&line, envp);
		write(fd[1], line, ft_strlen(line));
		free(line);
		line = get_next_line(STDIN_FILENO);
	}
	close(fd[1]);
}
