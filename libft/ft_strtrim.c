/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierant <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 21:52:24 by mpierant          #+#    #+#             */
/*   Updated: 2024/12/16 21:52:28 by mpierant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

static int	ft_check(char c, char const *set)
{
	int	i;
	int	a;

	a = 0;
	i = 0;
	while (set[i] != '\0')
	{
		if (set[i] == c)
			a = 1;
		i++;
	}
	return (a);
}

static int	ft_dim(char const *s1, char const *set)
{
	int	i;
	int	dim;

	dim = ft_strlen(s1);
	i = 0;
	while (ft_check(s1[i], set) && dim > 0)
	{
		dim--;
		i++;
	}
	while (s1[i] != '\0')
		i++;
	i--;
	while (ft_check(s1[i], set) && dim > 0)
	{
		dim--;
		i--;
	}
	return (dim);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	int		i;
	int		j;
	int		dim;
	char	*str;

	dim = ft_dim(s1, set);
	str = (char *)malloc((dim + 1) * sizeof(char));
	if (str == NULL)
	{
		free(str);
		return (NULL);
	}
	i = 0;
	j = 0;
	while (ft_check(s1[i], set))
		i++;
	while (j < dim)
	{
		str[j] = s1[i];
		j++;
		i++;
	}
	str[j] = '\0';
	return (str);
}
