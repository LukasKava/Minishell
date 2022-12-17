/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_II.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbiederm <pbiederm@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/18 00:17:36 by pbiederm          #+#    #+#             */
/*   Updated: 2022/12/18 00:19:26 by pbiederm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"../../includes/minishell.h"

int	in_redirection_this_node(t_chunk **salt)
{
	t_chunk	*element;

	element = *salt;
	if (element->in_f != NULL)
	{
		return (1);
	}
	else
	{
		return (0);
	}
}

int	in_redirection_next_node(t_chunk **salt)
{
	t_chunk	*element;

	element = *salt;
	if (element->next != NULL && element->next->in_f != NULL)
	{
		return (1);
	}
	else
	{
		return (0);
	}
}
