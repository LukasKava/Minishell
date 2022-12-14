/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_list.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkaufman <rkaufman@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/02 21:55:11 by rkaufman          #+#    #+#             */
/*   Updated: 2022/04/07 10:21:24 by rkaufman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_tester.h"

t_result	*ft_lstnew(char *content)
{
	t_result	*new_element;

	new_element = (t_result *) malloc(sizeof(t_result));
	if (!new_element)
		return (NULL);
	new_element->result = ft_string_dup(content);
	new_element->next = NULL;
	return (new_element);
}

t_result	*ft_lstlast(t_result *lst)
{
	while (lst)
	{
		if (!lst->next)
			return (lst);
		lst = lst->next;
	}
	return (lst);
}

void	ft_lstadd_back(t_result **lst, t_result *new)
{
	if (!*lst)
		*lst = new;
	else
		ft_lstlast(*lst)->next = new;

}

void	ft_delete_list(t_result **lst)
{
	t_result	*tmp;

	while (*lst)
	{
		tmp = *lst;
		*lst = (*lst)->next;
		if (tmp->result)
			free(tmp->result);
		tmp->result = NULL;
		free(tmp);
		tmp = NULL;
	}
	*lst = NULL;
}
