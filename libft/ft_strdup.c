/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RIV_ft_strdup.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierant <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 23:18:04 by mpierant          #+#    #+#             */
/*   Updated: 2024/12/12 23:18:08 by mpierant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

char	*ft_strdup(const char *s)
{
	char	*dup;
	int		i;

	i = 0;
	while (s[i] != '\0')
		i++;
	dup = (char *)malloc((i + 1) * sizeof(char));
	if (dup == NULL)
		return (NULL);
	i = 0;
	while (s[i] != '\0')
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}
