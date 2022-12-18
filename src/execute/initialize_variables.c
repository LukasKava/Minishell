/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize_variables.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbiederm <pbiederm@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/10 17:12:43 by pbiederm          #+#    #+#             */
/*   Updated: 2022/12/18 13:51:54 by pbiederm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"../../includes/minishell.h"

t_vars	*initialize_vars(t_chunk **salt)
{
	t_vars	*vars;

	vars = malloc(sizeof(*vars));
	vars->num_cmd = count_command_number(salt);
	vars->number_of_infiles = 0;
	vars->number_of_outfiles = 0;
	vars->pid = -1;
	vars->input_fd = -1;
	vars->output_fd = -1;
	vars->pipe_group = 0;
	vars->save_stdin = -1;
	vars->save_stdout = -1;
	vars->capture_exit_flag = -1;
	vars->capture_redirection_error = 0;
	return (vars);
}

int	count_command_number(t_chunk **salt)
{
	t_chunk	*elements;
	int		num_cmds;

	elements = *salt;
	num_cmds = 0;
	while (elements)
	{
		if (elements->indentifier == CMD_BLOCK || \
		elements->indentifier == BUILT_IN_BLOCK)
			num_cmds++;
		elements = elements->next;
	}
	return (num_cmds);
}
