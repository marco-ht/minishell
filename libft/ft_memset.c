/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierant <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 21:08:39 by mpierant          #+#    #+#             */
/*   Updated: 2024/12/12 21:08:45 by mpierant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include <stdio.h>
#include <stddef.h>

void	*ft_memset(void *s, int c, size_t n)
{
	size_t			i;
	unsigned char	*a;

	a = (unsigned char *)s;
	i = 0;
	while (i < n)
	{
		a[i] = (unsigned char)c;
		i++;
	}
	return (s);
}

/*int	main(void)
{
	int	c;
	size_t	n;
	void	*s;

	c = 'A';
	n = 3;
	s = "ciao";
	printf("risultato: %s\n", char *(ft_memset(s, c, n)));
}*/
