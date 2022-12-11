/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_fork.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbiederm <pbiederm@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/27 11:52:18 by pbiederm          #+#    #+#             */
/*   Updated: 2022/12/11 13:40:49 by pbiederm         ###   ########.fr       */
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
	last_cmd_output(&element, vars, i);
	first_cmd_input(&element, i);
	set_pipe_io(&element, vars, i);
	redirect_io(&element, vars);
	redirect_out(&element, vars);
	redirect_in(&element, vars);
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
		elements->fd[1] = save_std_out;
		elements->fd[0] = save_std_in;
	
		if (pipe_this_node(&elements))
			if(pipe(elements->fd) == -1)
				write(2, "Error while creating pipe\n", 27);
		if ((elements->indentifier == CMD_BLOCK &&
		elements->command_path != NULL) ||
		elements->indentifier == BUILT_IN_BLOCK)
		{
			manage_fd(&elements, vars, i);
			echo_handle(&elements);
			cd_handle(&elements, data->env);
			pwd_handle(&elements);
			env_handle(&elements, data->env);
			export_handle(&data->exp_l, &data->env, &elements, elements->fd[1]);
			// int builtins_unset(t_env **exp_l, t_env **env_l, char **line)
			unset_handle(&data->exp_l, &data->env, &elements);
			pids = fork();
			if (pids == -1)
			{
				freeing_chunks(salt, &data->info);
				free(vars);
				write(2, "Error while creating process\n", 30);
			}
			if (pids == 0)
			{
				if(elements->indentifier == BUILT_IN_BLOCK)
					exit(EXIT_SUCCESS);
				run(elements, &data->info, envp, vars);
			}
		}
		dup2(save_std_in, STDIN_FILENO);
		dup2(save_std_out, STDOUT_FILENO);
		if (i != 0)
		{
			close(elements->prev->fd[0]);
			close(elements->prev->fd[1]);
		}
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