/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbiederm <pbiederm@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/10 17:51:48 by pbiederm          #+#    #+#             */
/*   Updated: 2022/12/12 21:00:27 by pbiederm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"../../includes/minishell.h"

void	set_pipe_io(t_chunk **salt, t_vars *vars, int i)
{
	t_chunk	*element;
	
	element = *salt;
	if (i != vars->num_cmd - 1 && pipe_this_node(&element))
	{
		dup2(element->fd[1], STDOUT_FILENO);
	}
	if (i != 0 && pipe_last_node(&element))
	{
		dup2(element->prev->fd[0], STDIN_FILENO);
		close(element->prev->fd[1]);
		close(element->prev->fd[0]);
	}
}

int	pipe_this_node(t_chunk **salt)
{
	t_chunk	*elements;
	
	elements = *salt;
	if(elements->next != NULL &&
	elements->out_f == NULL &&
	elements->next->in_f == NULL)
		return(1);
	else
		return(0);
}

int	pipe_last_node(t_chunk **salt)
{
	t_chunk	*elements;
	
	elements = *salt;
	if(elements->prev != NULL &&
	elements->prev->out_f == NULL &&
	elements->in_f == NULL)
		return(1);
	else
		return(0);
}
