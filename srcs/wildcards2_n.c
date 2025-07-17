/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierant & sfelici <marvin@student.42ro    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 16:10:29 by mpierant &        #+#    #+#             */
/*   Updated: 2025/07/17 17:23:22 by mpierant &       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// match '*'
int	match_star(const char *pat, const char *name)
{
	if (*pat == '\0')
		return (*name == '\0');
	if (*pat == '*')
	{
		while (*pat == '*')
			pat++;
		if (*pat == '\0')
			return (1);
		while (*name)
		{
			if (match_star(pat, name))
				return (1);
			name++;
		}
		return (0);
	}
	if (*pat == *name)
		return (match_star(pat + 1, name + 1));
	return (0);
}

char	*ft_strndup(const char *s, size_t n)
{
	char	*dup;
	size_t	len;
	size_t	i;

	len = 0;
	while (s[len] && len < n)
		len++;
	dup = malloc(len + 1);
	if (!dup)
		return (NULL);
	i = 0;
	while (i < len)
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

static char	*ft_def_return(char *pattern, char *matched, int *allocated)
{
	if (matched)
	{
		*allocated = 1;
		return (matched);
	}
	return (pattern);
}

/**
 * Se `pattern` contiene un asterisco, cerca nel CWD
 * il primo nome che matcha e restituisce una strdup()
 * di quel nome. Altrimenti torna semplicemente `pattern`.
 */
char	*expand_redirect_glob(char *pattern, int *allocated)
{
	DIR				*d;
	struct dirent	*e;
	char			*matched;

	if (!ft_strchr(pattern, '*'))
		return (pattern);
	matched = NULL;
	d = opendir(".");
	if (!d)
		return (pattern);
	e = readdir(d);
	while (e)
	{
		if (match_star(pattern, e->d_name))
		{
			matched = ft_strdup(e->d_name);
			break ;
		}
		e = readdir(d);
	}
	closedir(d);
	return (ft_def_return(pattern, matched, allocated));
}

// bubble sort per ordinamento alfabetico
void	sort_matches(char **matches, int count)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (i < count - 1)
	{
		j = 0;
		while (j < count - i - 1)
		{
			if (strcmp(matches[j], matches[j + 1]) > 0)
			{
				tmp = matches[j];
				matches[j] = matches[j + 1];
				matches[j + 1] = tmp;
			}
			j++;
		}
		i++;
	}
}
