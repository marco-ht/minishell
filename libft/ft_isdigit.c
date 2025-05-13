/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isdigit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierant <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 19:21:51 by mpierant          #+#    #+#             */
/*   Updated: 2024/12/11 16:23:37 by mpierant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include <stdio.h>

int	ft_isdigit(int c)
{
	int	r;

	r = 0;
	if (c >= '0' && c <= '9')
	{
		r = 1;
	}
	return (r);
}

/* alternativa piu compatta

	return(c >= '0' && c <= '9')

	gestione caso E0F?
	if( c == E0F)
	{
		return (0);
	}
*/

/*int	main(void)
{
	int	c;

	c = '(';
	printf("risultato: %i\n", ft_isdigit(c));
}*/
