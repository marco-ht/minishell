/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser1c_n.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierant & sfelici <marvin@student.42ro    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 22:54:40 by mpierant &        #+#    #+#             */
/*   Updated: 2025/07/20 15:14:33 by mpierant &       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	handle_quoted_string(t_token_data *data, char **out_write,
		char quote, int *ret)
{
	if (*ret == 'a')
	{
		if (quote == '\'')
			*ret = 's';
		else
			*ret = 'd';
	}
	while (data->s < data->es && *data->s != quote)
		*(*out_write)++ = *data->s++;
	if (data->s < data->es)
		data->s++;
}

void	handle_argument_loop(t_token_data *data, char **out_write, int *ret)
{
	char	quote;

	while (data->s < data->es && !ft_strchr(data->whitespace, *data->s)
		&& !ft_strchr(data->symbols, *data->s))
	{
		if (*data->s == '\'' || *data->s == '"')
		{
			quote = *data->s++;
			handle_quoted_string(data, out_write, quote, ret);
		}
		else if (*data->s == '\\' && data->s + 1 < data->es)
		{
			data->s++;
			*(*out_write)++ = *data->s++;
			*ret = 's';
		}
		else
			*(*out_write)++ = *data->s++;
	}
}

int	handle_argument_token(t_token_data *data)
{
	char	*start;
	char	*out_write;

	start = data->s;
	out_write = data->s;
	data->ret = 'a';
	handle_argument_loop(data, &out_write, &data->ret);
	if (data->q)
		*data->q = start;
	if (data->eq)
		*data->eq = out_write;
	return (data->ret);
}

int	dispatch_token(t_token_data *data)
{
	int	ret;

	ret = *data->s;
	if (*data->s == '\0')
		return (0);
	else if (*data->s == '(' || *data->s == ')')
		data->s++;
	else if (*data->s == ';')
		data->s++;
	else if (*data->s == '|')
		ret = handle_pipe_token(data);
	else if (*data->s == '&')
		ret = handle_and_token(data);
	else if (*data->s == '>')
		ret = handle_redirect_out_token(data);
	else if (*data->s == '<')
		ret = handle_redirect_in_token(data);
	else
		ret = handle_argument_token(data);
	return (ret);
}

void	skip_whitespace_update_ps(t_token_data *data,
		char **ps)
{
	while (data->s < data->es && ft_strchr(data->whitespace, *data->s))
		data->s++;
	*ps = data->s;
}
