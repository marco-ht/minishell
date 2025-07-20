/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runcmd2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierant & sfelici <marvin@student.42ro    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 15:29:55 by mpierant &        #+#    #+#             */
/*   Updated: 2025/07/21 00:20:28 by mpierant &       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	update_exit_status(int status, int *p_last_exit_status)
{
	*p_last_exit_status = status;
}

static int	check_pwd_echo(t_execcmd *ecmd, int *p_last_exit_status)
{
	int	exit_status;

	if (ft_strncmp(ecmd->argv[0], "pwd", 4) == 0)
	{
		exit_status = builtin_pwd();
		return (update_exit_status(exit_status, p_last_exit_status),
			exit_status);
	}
	else if (ft_strncmp(ecmd->argv[0], "echo", 5) == 0)
	{
		exit_status = builtin_echo(ecmd);
		return (update_exit_status(exit_status, p_last_exit_status),
			exit_status);
	}
	return (-1);
}

static int	check_cd_exit_env(t_execcmd *ecmd, char ***envp,
		int *p_last_exit_status)
{
	int	exit_status;

	if (ft_strncmp(ecmd->argv[0], "cd", 3) == 0)
	{
		exit_status = builtin_cd(ecmd, *envp);
		return (update_exit_status(exit_status, p_last_exit_status),
			exit_status);
	}
	else if (ft_strncmp(ecmd->argv[0], "exit", 5) == 0)
	{
		exit_status = builtin_exit(ecmd);
		return (update_exit_status(exit_status, p_last_exit_status),
			exit_status);
	}
	else if (ft_strncmp(ecmd->argv[0], "env", 4) == 0)
	{
		exit_status = builtin_env(*envp);
		return (update_exit_status(exit_status, p_last_exit_status),
			exit_status);
	}
	return (-1);
}

static int	check_export_unset(t_execcmd *ecmd, char ***envp,
		int *p_last_exit_status)
{
	int	exit_status;

	if (ft_strncmp(ecmd->argv[0], "export", 7) == 0)
	{
		exit_status = builtin_export(ecmd, envp);
		return (update_exit_status(exit_status, p_last_exit_status),
			exit_status);
	}
	else if (ft_strncmp(ecmd->argv[0], "unset", 6) == 0)
	{
		exit_status = builtin_unset(ecmd, envp);
		return (update_exit_status(exit_status, p_last_exit_status),
			exit_status);
	}
	return (-1);
}

int	ft_check_builtin(t_execcmd *ecmd, char ***envp, int *p_last_exit_status)
{
	int	ret;

	if (ecmd->argv[0] == NULL)
		return (-1);
	ret = check_pwd_echo(ecmd, p_last_exit_status);
	if (ret != -1)
		return (ret);
	ret = check_cd_exit_env(ecmd, envp, p_last_exit_status);
	if (ret != -1)
		return (ret);
	ret = check_export_unset(ecmd, envp, p_last_exit_status);
	if (ret != -1)
		return (ret);
	return (-1);
}
