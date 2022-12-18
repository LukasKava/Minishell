/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process_do.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbiederm <pbiederm@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/18 19:45:33 by pbiederm          #+#    #+#             */
/*   Updated: 2022/12/18 19:46:44 by pbiederm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	child_process_do(t_chunk **salt, \
t_data *data, t_vars *vars, char **envp)
{
	t_chunk	*element;

	element = *salt;
	vars->save_stdout = dup(STDOUT_FILENO);
	vars->save_stdin = dup(STDIN_FILENO);
	pipe_fork(&element, data, envp, vars);
	restore_standard_io(vars);
}
