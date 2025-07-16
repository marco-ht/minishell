/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins5.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierant <mpierant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 11:52:19 by mpierant          #+#    #+#             */
/*   Updated: 2025/07/16 13:28:45 by mpierant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	builtin_export(t_execcmd *ecmd, char ***envp)
{
	int	i;

	if (!ecmd->argv[1])
		return (print_all_env(*envp));
	i = 1;
	while (ecmd->argv[i])
	{
		if (process_export_arg(ecmd->argv[i], envp))
			return (1);
		i++;
	}
	return (0);
}
