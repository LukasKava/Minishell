/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize_variables.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbiederm <pbiederm@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/10 17:12:43 by pbiederm          #+#    #+#             */
/*   Updated: 2022/12/11 14:38:56 by pbiederm         ###   ########.fr       */
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
	return(vars);
}

int	count_command_number(t_chunk **salt)
{
	t_chunk	*elements;
	int		num_cmds;
	
	elements = *salt;
	num_cmds = 0;
	while(elements)
	{
		if(elements->indentifier == CMD_BLOCK ||
		elements->indentifier == BUILT_IN_BLOCK)
			num_cmds++;
		elements = elements->next;
	}
	return(num_cmds);
}