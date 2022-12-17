/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_fork.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbiederm <pbiederm@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/27 11:52:18 by pbiederm          #+#    #+#             */
/*   Updated: 2022/12/17 23:31:29 by lkavalia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
	Still need to work on.
*/
#include "../../includes/minishell.h"

// void	get_exit_status(t_vars *vars, int status)
// {
// 	int	i;

// 	i = 0;
// 	while (i < vars->num_cmd)
// 	{
// 		if (WIFEXITED(status))
// 			g_errors.g_exit_status = WEXITSTATUS(status);
// 		i++;
// 	}
// }

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

void	no_fork_handle(t_chunk **salt, t_data *data, char **env)
{
	t_chunk	*element;

	element = *salt;
	echo_handle(&element);
	pwd_handle(&element);
	env_handle(&element, data->env);
	cd_handle(&element, data->env);
	export_handle(&data->exp_l, &data->env, &element, STDOUT_FILENO);
	unset_handle(&data->exp_l, &data->env, env, &element);
	exit_handle(data, &element);
}

void	built_in_handler(t_chunk **salt, t_data *data, t_vars *vars)
{
	t_chunk	*element;

	element = *salt;
	if (pipe_this_node(&element) && element->indentifier == BUILT_IN_BLOCK)
	{
		vars->pid = fork();
		if (vars->pid == -1)
		{
			g_errors.g_exit_status = 1;
			write(2, "Error while creating process\n", 30);
		}
		if (vars->pid == 0)
		{
			echo_handle(&element);
			pwd_handle(&element);
			env_handle(&element, data->env);
			vars->capture_exit_flag = 1;
			exit(EXIT_SUCCESS);
		}
	}
	else
		no_fork_handle(&element, data, env);
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
		vars->save_stdout = dup(STDOUT_FILENO);
		vars->save_stdin = dup(STDIN_FILENO);
		pipe_fork(&elements, data, envp, vars);
		restore_standard_io(vars);
		waitpid(vars->pid, &status, 0);
		signal(SIGINT, handle_sigint);
		vars->pipe_group++;
		elements = elements->next;
	}
	if (vars->capture_exit_flag > 0)
	{
		g_errors.g_exit_status = WEXITSTATUS(status);
		vars->capture_exit_flag = -1;
	}
	g_errors.bip = false;
	free(vars);
}	
