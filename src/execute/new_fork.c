/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_fork.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbiederm <pbiederm@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/27 11:52:18 by pbiederm          #+#    #+#             */
/*   Updated: 2022/12/12 18:17:06 by pbiederm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
Needs a simple command execute, to execute one command.
Needs status 126 when command is not found by access.
Closing file descriptors hen there is one command.

*/
#include	"../../includes/minishell.h"

void	get_exit_status(t_vars *vars,  int status);
void	manage_fd(t_chunk **salt, t_vars *vars, int i);

void	get_exit_status(t_vars *vars,  int status)
{
	int	i;

	i = 0;
	while(i < vars->num_cmd)
	{
		g_exit_status = WEXITSTATUS(status);
		i++;
	}
}

void manage_fd(t_chunk **salt, t_vars *vars, int i)
{
	t_chunk *element;

	element = *salt;
	empty_data_input(&element, i);
	empty_data_output(&element, vars, i);
	// last_cmd_output(&element, vars, i);
	// first_cmd_input(&element, i);
	redirect_in(&element, vars);
	redirect_io(&element, vars);
	redirect_out(&element, vars);
	set_pipe_io(&element, vars, i);
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

	int		pids;
	int		i;
	int		status;
	int		save_std_out;
	int		save_std_in;

	elements = *salt;
	vars = initialize_vars(salt);
	i = 0;
	while (elements)
	{
		save_std_out = dup(STDOUT_FILENO);
		save_std_in = dup(STDIN_FILENO);
		if (pipe_this_node(&elements))
			if(pipe(elements->fd) == -1)
				write(2, "Error while creating pipe\n", 27);
		manage_fd(&elements, vars, i);
		built_in_handler(&elements, data);
		if ((elements->indentifier == CMD_BLOCK &&
		elements->command_path != NULL))
		{
			pids = fork();
			if (pids == -1)
			{
				freeing_chunks(salt, &data->info);
				free(vars);
				write(2, "Error while creating process\n", 30);
			}
			if (pids == 0)
				run(elements, &data->info, envp, vars);
		}
		else if(elements->indentifier == CMD_BLOCK &&
		elements->command_path == NULL)
		{
			g_exit_status = 127;
			write(2, elements->arguments[0], strlen(elements->arguments[0]));
			write(2,": ", 3);
			write(2, "Write propper commands, eat healthy.\n", 38);
		}
		dup2(save_std_in, STDIN_FILENO);
		dup2(save_std_out, STDOUT_FILENO);
		close(save_std_in);
		close(save_std_out);
		waitpid(-1, &status, 0);
		get_exit_status(vars, status);
		i++;
		elements = elements->next;
	}
}
	// 	// If a command is not found, the child process created to execute it 
	// 	// returns a status of 127. If a command is found but is not executable, 
	// 	// the return status is 126. 

	// 	fprintf(stderr,"exit status: %d\n", g_exit_status);
	// 	fprintf(stderr, "Parrent waited for process pids[%d]\n", i);