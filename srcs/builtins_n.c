/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_n.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierant & sfelici <marvin@student.42ro    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 11:32:14 by mpierant          #+#    #+#             */
/*   Updated: 2025/07/16 13:58:38 by mpierant &       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*get_pwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		perror("minishell: cd");
	return (cwd);
}

int	builtin_pwd(void)
{
	char	*cwd;

	cwd = get_pwd();
	if (cwd)
	{
		printf("%s\n", cwd);
		free(cwd);
		return (0);
	}
	return (1);
}
