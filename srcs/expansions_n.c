/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierant & sfelici <marvin@student.42ro    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 16:30:00 by mpierant &        #+#    #+#             */
/*   Updated: 2025/07/16 16:42:35 by mpierant &       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	handle_variable_expansion(t_execcmd *ecmd, int i, char **envp)
{
	char	*temp_arg;

	temp_arg = ft_strdup(ecmd->argv[i]);
	perform_expansion(&temp_arg, envp);
	if (ecmd->allocated[i])
		free(ecmd->argv[i]);
	ecmd->argv[i] = temp_arg;
	ecmd->allocated[i] = 1;
}

void	expand_variables(t_execcmd *ecmd, char **envp, int *p_last_exit_status)
{
	int	i;

	i = 0;
	while (ecmd->argv[i])
	{
		if (ecmd->qtype[i] != 's' && ft_strchr(ecmd->argv[i], '$'))
		{
			handle_exit_status_expansion(ecmd, i, p_last_exit_status);
			handle_variable_expansion(ecmd, i, envp);
		}
		i++;
	}
}

char	*expand_exit_status(char *str, int *p_last_exit_status)
{
	char	*result;
	char	*pos;
	char	*exit_str;
	int		len;

	if (!str)
		return (NULL);
	pos = ft_strnstr(str, "$?", ft_strlen(str));
	if (!pos)
		return (str);
	exit_str = ft_itoa(*p_last_exit_status);
	len = ft_strlen(str) - 2 + ft_strlen(exit_str) + 1;
	result = malloc(len);
	if (!result)
		return (NULL);
	ft_strlcpy(result, str, pos - str + 1);
	result[pos - str] = '\0';
	ft_strlcat(result, exit_str, len);
	ft_strlcat(result, pos + 2, len);
	return (result);
}
