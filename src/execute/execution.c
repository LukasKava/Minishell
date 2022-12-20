/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbiederm <pbiederm@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/17 11:13:34 by pbiederm          #+#    #+#             */
/*   Updated: 2022/12/20 12:37:56 by pbiederm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	fork_error(void)
{
	g_errors.g_exit_status = 1;
	perror(" ");
}

void	command_error(t_chunk **salt, t_vars *vars)
{
	t_chunk	*element;

	element = *salt;
	g_errors.g_exit_status = 126;
	vars->capture_exit_flag = -1;
	write(2, element->arguments[0], strlen(element->arguments[0]));
	write(2, ": ", 3);
	write(2, "Write propper commands, eat healthy.\n", 38);
}

void	restore_standard_io(t_vars *vars)
{
	dup2(vars->save_stdin, STDIN_FILENO);
	dup2(vars->save_stdout, STDOUT_FILENO);
	close(vars->save_stdin);
	close(vars->save_stdout);
}

void	child_process(t_chunk **salt, t_vars *vars, char **envp)
{
	t_chunk	*element;

	element = *salt;
	vars->pid = fork();
	if (vars->pid == -1)
		fork_error();
	if (vars->pid == 0)
		run(element, envp);
}

void	pipe_fork(t_chunk **salt, t_data *data, char **envp, t_vars *vars)
{
	t_chunk	*element;

	element = *salt;
	if (pipe_this_node(&element))
		if (pipe(element->fd) == -1)
			pipe_error_execute();
	manage_fd(&element, vars);
	built_in_handler(&element, data, vars, envp);
	if (element->indentifier == CMD_BLOCK)
		child_process(&element, vars, envp);
}
