/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser1a.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierant & sfelici <marvin@student.42ro    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 19:48:33 by mpierant &        #+#    #+#             */
/*   Updated: 2025/07/18 22:55:37 by mpierant &       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	gettoken(char **ps, char *es, char **q, char **eq)
{
	t_token_data	data;
	int				retval;

	data.s = *ps;
	data.es = es;
	data.q = q;
	data.eq = eq;
	data.whitespace = " \t\r\n\v";
	data.symbols = "<|>&;()";
	data.ret = 0;
	set_positions(&data);
	retval = dispatch_token(&data);
	skip_trailing_whitespace_and_update_ps(&data, ps);
	return (retval);
}
