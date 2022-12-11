/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_fork.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbiederm <pbiederm@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/27 11:52:18 by pbiederm          #+#    #+#             */
/*   Updated: 2022/12/11 11:34:57 by pbiederm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
Needs a simple command execute, to execute one command.
*/
#include	"../../includes/minishell.h"

void	get_exit_status(t_vars *vars,  int status);
void	last_cmd_output(t_chunk	**salt, t_vars *vars, int i);
void	first_cmd_input(t_chunk **salt, int i);

void	last_cmd_output(t_chunk	**salt, t_vars *vars, int i)
{
	t_chunk	*element;

	element = *salt;
	if (i == vars->num_cmd - 1 &&
	(!(out_redirection_this_node(&element))))
	{
		dup2(element->fd[1], STDOUT_FILENO);
	}
}

void	first_cmd_input(t_chunk **salt, int i)
{
	t_chunk	*element;
	
	element = *salt;
	if (i == 0 &&
	(!(in_redirection_this_node(&element))))
	{
		dup2(element->fd[0], STDIN_FILENO);
	}
}

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
				empty_data_input(&elements, i);
				empty_data_output(&elements, vars, i);
				last_cmd_output(&elements, vars, i);
				first_cmd_input(&elements, i);
				set_pipe_io(&elements, vars, i);
				redirect_io(&elements, vars);
				redirect_out(&elements, vars);
				redirect_in(&elements, vars);
				echo_handle(&elements);
				cd_handle(&elements, data->env);
				pwd_handle(&elements);
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
				run(elements, &data->info, envp);
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