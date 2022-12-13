/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_fork.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/27 11:52:18 by pbiederm          #+#    #+#             */
/*   Updated: 2022/12/13 13:38:32 by lkavalia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
Needs a simple command execute, to execute one command.
Needs status 126 when command is not found by access.
Closing file descriptors hen there is one command.
Wifeexited afte waitpid.
Install norminett and make it in accordance with the norm.
Check cases and check for memory leaks.
Add error handling.
*/
#include	"../../includes/minishell.h"

void	get_exit_status(t_vars *vars);
void	manage_fd(t_chunk **salt, t_vars *vars);

void	get_exit_status(t_vars *vars)
{
	int	i;

	i = 0;
	while(i < vars->num_cmd)
	{
		g_exit_status = WEXITSTATUS(g_exit_status);
		i++;
	}
}

void manage_fd(t_chunk **salt, t_vars *vars)
{
	t_chunk *element;

	element = *salt;
	empty_data_input(&element, vars);
	empty_data_output(&element, vars);
	redirect_in(&element, vars);
	redirect_out(&element, vars);
	set_pipe_io(&element, vars);
}

void	built_in_handler(t_chunk **salt, t_data *data)
{
	t_chunk		*element;

	element = *salt;
	echo_handle(&element);
	cd_handle(&element, data->env);
	pwd_handle(&element);
	env_handle(&element, data->env);
	export_handle(&data->exp_l, &data->env, &element, STDOUT_FILENO);
	unset_handle(&data->exp_l, &data->env, &element);
	exit_handle(data, &element);
}

void	execute(t_chunk **salt, t_data *data, char	**envp)
{
	t_chunk	*elements;
	t_vars	*vars;

	elements = *salt;
	vars = initialize_vars(salt);
	while (elements)
	{
		vars->save_stdout = dup(STDOUT_FILENO);
		vars->save_stdin = dup(STDIN_FILENO);
		if (pipe_this_node(&elements))
			if(pipe(elements->fd) == -1)
			{
				g_exit_status = 1;
				write(2, "Error while creating pipe\n", 27);
			}
		manage_fd(&elements, vars);
		built_in_handler(&elements, data);
		if ((elements->indentifier == CMD_BLOCK &&
		elements->command_path != NULL))
		{
			vars->pid = fork();
			if (vars->pid == -1)
			{
				g_exit_status = 1;
				write(2, "Error while creating process\n", 30);
			}
			if (vars->pid == 0)
				run(elements, envp);
			else
				signal(SIGINT, SIG_IGN);
		}
		else if(elements->indentifier == CMD_BLOCK &&
		elements->command_path == NULL)
		{
			g_exit_status = 127;
			write(2, elements->arguments[0], strlen(elements->arguments[0]));
			write(2,": ", 3);
			write(2, "Write propper commands, eat healthy.\n", 38);
		}
		dup2(vars->save_stdin, STDIN_FILENO);
		dup2(vars->save_stdout, STDOUT_FILENO);
		close(vars->save_stdin);
		close(vars->save_stdout);
		waitpid(-1, &g_exit_status, 0);
		signal(SIGINT, handle_sigint);
		get_exit_status(vars);
		vars->pipe_group++;
		elements = elements->next;
	}
	free(vars);
}
