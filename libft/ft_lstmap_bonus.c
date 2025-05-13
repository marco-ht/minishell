/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierant <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 08:28:46 by mpierant          #+#    #+#             */
/*   Updated: 2024/12/21 10:18:51 by mpierant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

static t_list	*ft_new(t_list *lst, void *(*f)(void *))
{
	t_list	*node_ptr;

	node_ptr = (t_list *)malloc(sizeof(t_list));
	if (node_ptr == NULL)
		return (NULL);
	node_ptr->content = f(lst->content);
	if (node_ptr->content == NULL)
	{
		free(node_ptr);
		return (NULL);
	}
	node_ptr->next = NULL;
	return (node_ptr);
}

static t_list	*ft_del(t_list **head, void (*del)(void *))
{
	ft_lstclear(head, del);
	return (NULL);
}

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*newhead;
	t_list	*newlst;
	t_list	*oldlst;

	if (lst == NULL || (*f) == NULL)
		return (NULL);
	newhead = ft_new(lst, f);
	if (newhead == NULL)
		return (NULL);
	newlst = newhead;
	oldlst = lst->next;
	while (oldlst)
	{
		newlst->next = ft_new(oldlst, f);
		if (newlst->next == NULL)
			return (ft_del(&newhead, del));
		newlst = newlst->next;
		oldlst = oldlst->next;
	}
	return (newhead);
}
