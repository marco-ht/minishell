/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierant <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 20:30:11 by mpierant          #+#    #+#             */
/*   Updated: 2024/12/16 20:54:47 by mpierant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	dim;
	size_t	i;
	size_t	j;
	char	*str;

	dim = ft_strlen(s1) + ft_strlen(s2);
	str = (char *)malloc((dim + 1) * sizeof(char));
	if (str == NULL)
	{
		free(str);
		return (NULL);
	}
	j = 0;
	i = 0;
	while (s1[i] != '\0')
		str[j++] = s1[i++];
	i = 0;
	while (s2[i] != '\0')
		str[j++] = s2[i++];
	str[j] = '\0';
	return (str);
}
