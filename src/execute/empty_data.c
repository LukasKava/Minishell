/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   empty_data.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbiederm <pbiederm@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/11 11:32:53 by pbiederm          #+#    #+#             */
/*   Updated: 2022/12/23 13:32:07 by pbiederm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"../../includes/minishell.h"

/*Function for the edge case when there is no pipe input.*/
void	empty_data_input(t_chunk	**salt, t_vars *vars)
{
	t_chunk	*element;
	int		empty_input_fd;

	element = *salt;
	if (vars->pipe_group != 0 && \
	(!(in_redirection_this_node(&element))) && \
	(!(pipe_last_node(&element))))
	{
		empty_input_fd = open("/tmp/tmp_in.txt", \
		O_CREAT | O_RDWR | O_TRUNC, 0644);
		if (empty_input_fd == -1)
		{
			write(2, "Error while opening temporary input file\n", 42);
			g_errors.g_exit_status = 1;
		}
		dup2(empty_input_fd, STDIN_FILENO);
		close(empty_input_fd);
	}
}

void	empty_data_output(t_chunk	**salt, t_vars *vars)
{
	t_chunk	*element;
	int		empty_output_fd;

	element = *salt;
	if (vars->pipe_group != vars->num_cmd - 1 && \
	(!(out_redirection_this_node(&element))) && \
	(!(pipe_this_node(&element))) && \
	element->indentifier != ELSE_BLOCK)
	{
		empty_output_fd = open("/tmp/tmp_out.txt", \
		O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if (empty_output_fd == -1)
		{
			perror(":");
			write(2, "Error while opening temporary output file\n", 43);
			g_errors.g_exit_status = 1;
		}
		dup2(empty_output_fd, STDOUT_FILENO);
		close(empty_output_fd);
	}
}
