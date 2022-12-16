/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_fork.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbiederm <pbiederm@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/27 11:52:18 by pbiederm          #+#    #+#             */
/*   Updated: 2022/12/16 18:17:37 by pbiederm         ###   ########.fr       */
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
Might have to fork with built ins for pipes.
Actually you shouldn't fork on export, unset, cd and exit, 
as they pretty much need to change the state of the main process.
Builtins without pipes must be executed in the parent process.
Every operation that you call to exit should free all of the resources correctly, 
of course excluding the leaks from readline()
It actually sets the exit status
[08:31]
exit with number
Either to the number provided or to "2" if you input random characters instead
 https://github.com/VladDrag/Minishell_Documentation
 Hello everyone, does anyone knows how to initialize the status variable used for waitpid() correct?
So that  valgrind is not complaining about it and the status works properly with WEXITSTATUS(status);
If I just initialize status = 0;
Then the waitpid(pid, &status, 0);
Always returns 0 as exit status for the child.
Adding exits.
Checking for memory leaks.
< infilea ls -l | wc -l > out
exit statuses from the built ins
*/
#include	"../../includes/minishell.h"

void	get_exit_status(t_vars *vars);
void	manage_fd(t_chunk **salt, t_vars *vars);

void	get_exit_status(t_vars *vars)
{
	int	i;

	i = 0;
	while (i < vars->num_cmd)
	{
		g_exit_status = WEXITSTATUS(g_exit_status);
		i++;
	}
}

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

// void	no_fork_builtins(t_chunk **salt, t_data *data)
// {
	
// }

void	built_in_handler(t_chunk **salt, t_data *data, char **env, t_vars *vars)
{
	t_chunk		*element;

	element = *salt;
	if (pipe_this_node(&element) && element->indentifier == BUILT_IN_BLOCK)
	{
		vars->pid = fork();
		if (vars->pid == -1)
		{
			g_exit_status = 1;
			write(2, "Error while creating process\n", 30);
		}
		if (vars->pid == 0)
		{
			echo_handle(&element);
			pwd_handle(&element);
			env_handle(&element, data->env);
			exit (EXIT_SUCCESS);
		}
	}
	else
	{
		echo_handle(&element);
		pwd_handle(&element);
		env_handle(&element, data->env);
		cd_handle(&element, data->env);
		export_handle(&data->exp_l, &data->env, &element, STDOUT_FILENO);
		unset_handle(&data->exp_l, &data->env, env, &element);
		exit_handle(data, &element);
	}
}

void	execute(t_chunk **salt, t_data *data, char	**envp)
{
	t_chunk	*elements;
	t_vars	*vars;

	elements = *salt;
	vars = initialize_vars(salt);
	// signal(SIGINT, handle_child);
	while (elements && bip == false)
	{
		signal(SIGINT, handle_child);
		vars->save_stdout = dup(STDOUT_FILENO);
		vars->save_stdin = dup(STDIN_FILENO);
		if (pipe_this_node(&elements))
		{
			if(pipe(elements->fd) == -1)
			{
				g_exit_status = 1;
				perror(" ");
			}
		}
		manage_fd(&elements, vars);
		built_in_handler(&elements, data, envp, vars);
		if ((elements->indentifier == CMD_BLOCK)) 
		{
			vars->pid = fork();
			if (vars->pid == -1)
			{
				g_exit_status = 1;
				perror(" ");
			}
			if (vars->pid == 0)
			{
				// run(elements, envp, data, vars);
				// if (bip == true)
				// {
				// 	write(2, "hello\n", 7);
				// 	exit(1);
				// 	break;
		  	// }
			  //	write(1, "run\n", 5);
        run(elements, envp);
			}
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
	bip = false;
	free(vars);
}
