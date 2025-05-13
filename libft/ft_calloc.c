/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierant <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 23:02:42 by mpierant          #+#    #+#             */
/*   Updated: 2024/12/20 14:12:08 by mpierant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

void	*ft_calloc(size_t nmemb, size_t size)
{
	void			*pointer;
	size_t			i;
	unsigned char	*p;

	if (size != 0 && nmemb > ((size_t)-1) / size)
		return (NULL);
	pointer = malloc(nmemb * size);
	if (pointer == NULL)
	{
		free(pointer);
		return (NULL);
	}
	p = (unsigned char *)pointer;
	i = 0;
	while (i < nmemb * size)
	{
		p[i] = '\0';
		i++;
	}
	return (pointer);
}

// From man calloc
/*If the multiplication of nmemb and size would result in integer
		overflow, then calloc() returns an error. */
