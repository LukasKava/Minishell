/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_io.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbiederm <pbiederm@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/11 12:02:43 by pbiederm          #+#    #+#             */
/*   Updated: 2022/12/11 12:03:46 by pbiederm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"../../includes/minishell.h"

void	last_cmd_output(t_chunk	**salt, t_vars *vars, int i)
{
	t_chunk	*element;

	element = *salt;
	if (i == vars->num_cmd - 1 &&
	(!(out_redirection_this_node(&element))))
	{
		dup2(element->fd[1], STDOUT_FILENO);
	}
}

void	first_cmd_input(t_chunk **salt, int i)
{
	t_chunk	*element;
	
	element = *salt;
	if (i == 0 &&
	(!(in_redirection_this_node(&element))))
	{
		dup2(element->fd[0], STDIN_FILENO);
	}
}