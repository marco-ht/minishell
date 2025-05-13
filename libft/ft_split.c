/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierant <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 22:11:25 by mpierant          #+#    #+#             */
/*   Updated: 2024/12/16 22:38:29 by mpierant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

static int	ft_count(char const *s, char c)
{
	int	num;
	int	i;
	int	dim;

	num = 0;
	i = 0;
	while (s[i] != '\0')
	{
		while (s[i] != '\0' && s[i] == c)
			i++;
		dim = 0;
		while (s[i] != '\0' && s[i] != c)
		{
			dim++;
			i++;
		}
		if (dim > 0)
			num++;
	}
	return (num);
}

static void	ft_put(char *dest, char const *s, char c)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (s[i] != c && s[i] != '\0')
	{
		dest[j] = s[i];
		i++;
		j++;
	}
	dest[j] = '\0';
}

static int	ft_free(char **matrix, int j)
{
	int	i;

	i = 0;
	while (i < j)
	{
		free(matrix[i]);
		i++;
	}
	return (1);
}

static int	ft_fill(char **matrix, char const *s, char c, int i)
{
	int	dim;
	int	j;

	j = 0;
	while (s[i] != '\0')
	{
		while (s[i] != '\0' && s[i] == c)
			i++;
		dim = 0;
		while (s[i] != '\0' && s[i] != c)
		{
			dim++;
			i++;
		}
		if (dim > 0)
		{
			matrix[j] = (char *)malloc((dim + 1) * sizeof(char));
			if (matrix[j] == NULL)
				return (ft_free(matrix, j));
			ft_put(matrix[j++], &s[i - dim], c);
		}
	}
	return (0);
}

char	**ft_split(char const *s, char c)
{
	char	**matrix;
	int		num;
	int		check;

	if (!s)
		return (NULL);
	num = ft_count(s, c);
	matrix = (char **)malloc((num + 1) * sizeof(char *));
	if (matrix == NULL)
	{
		free(matrix);
		return (NULL);
	}
	matrix[num] = NULL;
	if (num == 0)
		return (matrix);
	check = ft_fill(matrix, s, c, 0);
	if (check == 1)
	{
		free(matrix);
		return (NULL);
	}
	return (matrix);
}
