/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_fork.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbiederm <pbiederm@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/27 11:52:18 by pbiederm          #+#    #+#             */
/*   Updated: 2022/12/19 08:47:51 by pbiederm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// void	child_process_do(t_chunk **salt,
// addinde *data, t_vars *vars, char **envp)
// {
// 	t_chunk	*element;

// 	element = *salt;
// 	vars->save_stdout = dup(STDOUT_FILENO);
// 	vars->save_stdin = dup(STDIN_FILENO);
// 	pipe_fork(&element, data, envp, vars);
// 	restore_standard_io(vars);
// }

// vars->save_stdout = dup(STDOUT_FILENO);
// vars->save_stdin = dup(STDIN_FILENO);
// pipe_fork(&elements, data, envp, vars);
// restore_standard_io(vars);

void	manage_fd(t_chunk **salt, t_vars *vars)
{
	t_chunk	*element;

	element = *salt;
	empty_data_input(&element, vars);
	empty_data_output(&element, vars);
	redirect_in(&element, vars);
	redirect_out(&element, vars);
	set_pipe_io(&element, vars);
}

void	no_fork_handle(t_chunk **salt, t_data *data, t_vars *vars)
{
	t_chunk	*element;

	element = *salt;
	echo_handle(&element, vars);
	pwd_handle(&element, vars);
	env_handle(&element, data->env, vars);
	cd_handle(&element, data->env, data->exp_l);
	export_handle(&data->exp_l, &data->env, &element, STDOUT_FILENO);
	unset_handle(&data->exp_l, &data->env, &element);
	exit_handle(data, &element);
}

void	built_in_handler_child(t_chunk **salt, t_data *data, t_vars *vars)
{
	t_chunk	*element;

	element = *salt;
	echo_handle(&element, vars);
	pwd_handle(&element, vars);
	env_handle(&element, data->env, vars);
	if (ft_strncmp(element->arguments[0], \
	"export", strlen("export")) == 0)
		export_handle(&data->exp_l, &data->env, \
		&element, STDOUT_FILENO);
	if (vars->builtin_exit_fail)
		exit(EXIT_FAILURE);
	exit(EXIT_SUCCESS);
}

void	built_in_handler(t_chunk **salt, t_data *data, t_vars *vars)
{
	t_chunk	*element;

	element = *salt;
	if ((pipe_this_node(&element) || pipe_last_node(&element)) && \
	element->indentifier == BUILT_IN_BLOCK)
	{
		vars->pid = fork();
		if (vars->pid == -1)
		{
			g_errors.g_exit_status = 1;
			write(2, "Error while creating process\n", 30);
		}
		if (vars->pid == 0)
			built_in_handler_child(&element, data, vars);
	}
	else
		no_fork_handle(&element, data, vars);
}

void	execute(t_chunk **salt, t_data *data, char **envp)
{
	t_chunk	*elements;
	t_vars	*vars;
	int		status;

	elements = *salt;
	vars = initialize_vars(salt);
	while (elements && g_errors.bip == false)
	{
		signal(SIGINT, handle_child);
		child_process_do(&elements, data, vars, envp);
		signal(SIGINT, handle_sigint);
		vars->pipe_group++;
		elements = elements->next;
	}
	if (vars->pipe_group)
		waitpid(vars->pid, &status, 0);
	while (--vars->pipe_group)
		waitpid(-1, NULL, 0);
	if (WIFEXITED(status) && !(vars->builtin_exit_fail))
	{
		g_errors.g_exit_status = WEXITSTATUS(status);
		vars->capture_redirection_error = 0;
	}
	g_errors.bip = false;
	free(vars);
}			
