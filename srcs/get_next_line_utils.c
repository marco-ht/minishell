/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierant <mpierant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 23:25:28 by mpierant          #+#    #+#             */
/*   Updated: 2025/06/11 21:52:15 by mpierant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

char	*ft_strchr(const char *s, int c)
{
	int	i;

	i = 0;
	while (s[i] != '\0')
	{
		if (s[i] == (char)c)
			return ((char *)(s + i));
		i++;
	}
	if (c == '\0')
		return ((char *)(s + i));
	return (NULL);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	dim;
	size_t	i;
	size_t	j;
	char	*str;

	if (s1 == NULL || s2 == NULL)
		return (NULL);
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

size_t	ft_strlen(const char *s)
{
	size_t	r;

	if (s == NULL)
		return (0);
	r = 0;
	while (s[r] != '\0')
	{
		r++;
	}
	return (r);
}
