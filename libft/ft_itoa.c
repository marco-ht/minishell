/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierant <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 16:00:17 by mpierant          #+#    #+#             */
/*   Updated: 2024/12/18 16:22:22 by mpierant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

//#include <stdio.h>

static int	ft_count(int n)
{
	unsigned int	num;
	int				dim;

	if (n == 0)
		return (1);
	dim = 0;
	if (n < 0)
	{
		dim++;
		num = -n;
	}
	else
		num = n;
	while (num > 0)
	{
		num /= 10;
		dim++;
	}
	return (dim);
}

static void	ft_put(int n, int dim, char *str)
{
	int				stop;
	unsigned int	num;

	if (n == 0)
	{
		str[0] = '0';
		str[1] = '\0';
		return ;
	}
	stop = 0;
	if (n < 0)
	{
		num = -n;
		str[0] = '-';
		stop++;
	}
	else
		num = n;
	while (dim >= stop)
	{
		str[dim] = (num % 10) + '0';
		num /= 10;
		dim--;
	}
	return ;
}

char	*ft_itoa(int n)
{
	char	*str;
	int		dim;

	dim = ft_count(n);
	str = (char *)malloc((dim + 1) * sizeof(char));
	if (str == NULL)
		return (NULL);
	str[dim] = '\0';
	dim--;
	ft_put(n, dim, str);
	return (str);
}

/*int	main(void)
{
	printf("%s\n", ft_itoa(12));
}*/
