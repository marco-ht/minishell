/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_issign.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierant <mpierant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 21:13:04 by mpierant          #+#    #+#             */
/*   Updated: 2025/04/02 01:55:00 by mpierant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_issign(int c)
{
	int	r;

	r = 0;
	if (c == '+' || c == '-')
	{
		r = 1;
	}
	return (r);
}
