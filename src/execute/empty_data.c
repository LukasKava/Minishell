/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   empty_data.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbiederm <pbiederm@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/11 11:32:53 by pbiederm          #+#    #+#             */
/*   Updated: 2022/12/11 12:07:11 by pbiederm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"../../includes/minishell.h"

/*Function for the edge case when there is no pipe input.*/
void empty_data_input(t_chunk	**salt, int i)
{
	t_chunk *element;

	element = *salt;
	if(i != 0 && (!(in_redirection_this_node(&element))) && 
	(!(pipe_last_node(&element))))
	{
		element->fd[0] = open("./includes/tmp_in.txt",\
		O_CREAT | O_RDWR |O_TRUNC , 0644);
		dup2(element->fd[0], STDIN_FILENO);
		// close(element->fd[0]);
	}
}

void empty_data_output(t_chunk	**salt, t_vars *vars, int i)
{
	t_chunk *element;

	element = *salt;
	if (i != vars->num_cmd - 1 && 
	(!(out_redirection_this_node(&element))) &&
	(!(pipe_this_node(&element))))
	{
		element->fd[1] = open("./includes/tmp_out.txt", O_CREAT | O_RDWR | O_TRUNC , 0644);
		dup2(element->fd[1], STDOUT_FILENO);
		// close(element->fd[1]);
	}
}