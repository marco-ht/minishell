/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser1b.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierant & sfelici <marvin@student.42ro    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 22:51:41 by mpierant &        #+#    #+#             */
/*   Updated: 2025/07/18 22:52:01 by mpierant &       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	set_positions(t_token_data *data)
{
	while (data->s < data->es && ft_strchr(data->whitespace, *data->s))
		data->s++;
	if (data->q)
		*data->q = data->s;
	if (data->eq)
		*data->eq = data->s;
}

int	handle_pipe_token(t_token_data *data)
{
	int	ret;

	ret = '|';
	data->s++;
	if (data->s < data->es && *data->s == '|')
	{
		ret = 'o';
		data->s++;
	}
	return (ret);
}

int	handle_and_token(t_token_data *data)
{
	int	ret;

	ret = '&';
	data->s++;
	if (data->s < data->es && *data->s == '&')
	{
		ret = 'e';
		data->s++;
	}
	return (ret);
}

int	handle_redirect_out_token(t_token_data *data)
{
	int	ret;

	ret = '>';
	data->s++;
	if (data->s < data->es && *data->s == '>')
	{
		ret = '+';
		data->s++;
	}
	return (ret);
}

int	handle_redirect_in_token(t_token_data *data)
{
	int	ret;

	ret = '<';
	data->s++;
	if (data->s < data->es && *data->s == '<')
	{
		ret = 'h';
		data->s++;
	}
	return (ret);
}
