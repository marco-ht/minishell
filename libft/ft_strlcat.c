/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierant <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 19:32:41 by mpierant          #+#    #+#             */
/*   Updated: 2024/12/12 20:05:29 by mpierant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	src_l;
	size_t	dst_l;

	src_l = 0;
	while (src[src_l] != '\0')
		src_l++;
	dst_l = 0;
	while (dst[dst_l] != '\0' && dst_l < size)
		dst_l++;
	if (dst_l >= size)
		return (size + src_l);
	i = 0;
	while (src[i] != '\0' && dst_l + i < size - 1)
	{
		dst[dst_l + i] = src[i];
		i++;
	}
	dst[dst_l + i] = '\0';
	return (dst_l + src_l);
}
