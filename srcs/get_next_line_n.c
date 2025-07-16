/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierant <mpierant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 23:25:01 by mpierant          #+#    #+#             */
/*   Updated: 2025/06/11 21:52:10 by mpierant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 3
#endif

#include "../includes/minishell.h"

static char	*ft_addbuffer(char *str, char *buffer)
{
	char	*new_str;

	if (!str || !buffer)
		return (NULL);
	new_str = ft_strjoin(str, buffer);
	free(str);
	return (new_str);
}

static char	*ft_parse(char *str, int fd, int *pn)
{
	char	*buffer;

	buffer = (char *)ft_calloc(BUFFER_SIZE + 1, sizeof(char));
	if (buffer == NULL)
		return (free(str), NULL);
	*pn = 1;
	while (*pn > 0)
	{
		*pn = read(fd, buffer, BUFFER_SIZE);
		if (*pn == -1)
		{
			free(str);
			return (free(buffer), NULL);
		}
		buffer[*pn] = '\0';
		str = ft_addbuffer(str, buffer);
		if (str == NULL)
			return (free(buffer), NULL);
		if (ft_strchr(buffer, '\n'))
			break ;
	}
	free(buffer);
	return (str);
}

static char	*ft_get_line(char *str)
{
	int		i;
	char	*str_def;

	i = 0;
	if (str == NULL || str[i] == '\0')
		return (NULL);
	while (str[i] != '\0' && str[i] != '\n')
		i++;
	str_def = (char *)ft_calloc(i + 2, sizeof(char));
	if (str_def == NULL)
		return (NULL);
	i = 0;
	while (str[i] != '\0' && str[i] != '\n')
	{
		str_def[i] = str[i];
		i++;
	}
	if (str[i] == '\n')
		str_def[i] = '\n';
	return (str_def);
}

static char	*ft_afterpart(char *str)
{
	int		i;
	int		j;
	char	*afterpart;

	if (str == NULL)
		return (NULL);
	i = 0;
	while (str[i] != '\0' && str[i] != '\n')
		i++;
	if ((str[i] == '\0') || (str[i] == '\n' && str[i + 1] == '\0'))
		return (free(str), NULL);
	afterpart = (char *) ft_calloc(ft_strlen(str) - i + 1, sizeof(char));
	if (afterpart == NULL)
		return (NULL);
	i++;
	j = 0;
	while (str[i] != '\0')
	{
		afterpart[j] = str[i];
		i++;
		j++;
	}
	free(str);
	return (afterpart);
}

char	*get_next_line(int fd)
{
	static char	*str;
	char		*str_def;
	int			n;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (free(str), str = NULL, NULL);
	if (str == NULL)
	{
		str = (char *) ft_calloc(1, sizeof(char));
		if (str == NULL)
			return (NULL);
	}
	n = 0;
	if (!ft_strchr(str, '\n'))
		str = ft_parse(str, fd, &n);
	if (str == NULL)
		return (NULL);
	str_def = ft_get_line(str);
	str = ft_afterpart(str);
	if ((str_def == NULL) && n == 0)
		return (free(str), str = NULL, NULL);
	return (str_def);
}

//|| ft_strncmp(str, "\n\0", 2)

/*#include <stdio.h>//commentare
#include <fcntl.h>//commentare

int	main(void)
{
	int	fd;

	fd = open("testo.txt", O_RDONLY);
	if(fd)
	{
		printf("BUFFER SIZE = %i\n\n\n", BUFFER_SIZE);
		printf("1: %s", get_next_line(fd));
		printf("2: %s", get_next_line(fd));
		printf("3: %s", get_next_line(fd));
		printf("4: %s", get_next_line(fd));
		printf("5: %s", get_next_line(fd));
		printf("6: %s", get_next_line(fd));
		printf("7: %s", get_next_line(fd));
		printf("8: %s", get_next_line(fd));
		printf("9: %s", get_next_line(fd));
		printf("10: %s", get_next_line(fd));
		printf("11: %s", get_next_line(fd));
		printf("12: %s", get_next_line(fd));
		close(fd);
	}
}*/
