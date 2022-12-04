/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_fork.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbiederm <pbiederm@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/27 11:52:18 by pbiederm          #+#    #+#             */
/*   Updated: 2022/12/04 17:41:07 by pbiederm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"../../includes/minishell.h"

int	**create_pipes(t_chunk **salt, t_info *info, t_vars	*vars)
{
	t_chunk	*elements;
	int		num_pipes;
	int		**ret_table;

	num_pipes = 0;
	ret_table = calloc(9999, sizeof(int*));
	while (num_pipes < 9999)
	{
		ret_table[num_pipes] = calloc(2, sizeof(int));
		num_pipes++;
	}
	num_pipes = 0;
	elements = *salt;
	while (elements)
	{
		if(elements->indentifier == CMD_BLOCK)
		{
			if(pipe(ret_table[num_pipes]) == -1)
			{
				freeing_chunks(salt, info);
				write(2, "Error while making pipes\n", 25);
				return(NULL);
			}
			fprintf(stderr, "created pipes[%d]\n", num_pipes);
		num_pipes++;
		}
		elements = elements->next;
	}
	vars->num_cmd = num_pipes;
	fprintf(stderr,"vars.num_cmd = %d\n", vars->num_cmd);
	elements = *salt;
	return(ret_table);
}

void miner_closing_pipe(int **pipes, t_vars *vars, int i)
{
	int	j;

	j = 0;
	while (j < vars->num_cmd)
	{
		if (i != j)
		{
			close(pipes[j][0]);
			fprintf(stderr, "closed pipes[%d][0]\n", j);
		}
		if(i + 1 != j)
		{
			close(pipes[j][1]);
			fprintf(stderr, "closed pipes[%d][1]\n", j);
		}
		j++;
	}
}

void overseer_closing_pipe(int **pipes, t_vars *vars)
{
	int	k;

	k = 0;
	while (k < vars->num_cmd)
	{
		close(pipes[k][0]);
		fprintf(stderr, "parrent closed pipes[%d][0]\n", k);
		close(pipes[k][1]);
		fprintf(stderr, "parrent closed pipes[%d][1]\n", k);
		k++;
	}
	
}

// void	file_descriptor_table(int i, t_chunk **salt, )

void	execute(t_chunk **salt, t_info *info, char	**envp)
{
	t_chunk	*elements;
	t_vars	*vars;
	int		**pipes;
	int		pids;
	int		i;
	int		number_of_infiles;
	int		number_of_outfiles;
	int		status;
	// int		fd_tmp_out;
	// int		fd_tmp_in;

	elements = *salt;
	fprintf(stderr, "the command path%s\n", (*salt)->command_path);
	i = 0;
	number_of_infiles = 0;
	number_of_outfiles = 0;
	vars = malloc(sizeof(*vars));
	pipes = create_pipes(salt, info, vars);
	fprintf(stderr,"vars.num_cmd = %d\n", vars->num_cmd);
	while(elements)
	{
		elements->fd_out = dup(1);
		elements->fd_in = dup(0);
		// elements->fd_out = dup(1);
		fprintf(stderr, "Number of assignment 1Ama Assigning fd_out, fd_out: %d\n", elements->fd_out);
		if (i == 0)
		{
			
			while(elements->in_f != NULL && elements->in_f[number_of_infiles].name != NULL && elements->in_f[number_of_infiles].type == INPUT_F)
			{
				fprintf(stderr, "Number of assignment 2 Assigning fd_out, fd_out: %d\n", elements->fd_out);
				elements->fd_in = open(elements->in_f[number_of_infiles].name, O_RDONLY);
				fprintf(stderr, "string of files [%d]: %s\n", number_of_infiles, elements->in_f[number_of_infiles].name);
				number_of_infiles++;
			}
			number_of_infiles = 0;
			while(elements->in_f != NULL && elements->in_f[number_of_infiles].type == DELIMITOR)
			{
				fprintf(stderr, "Number of assignment 3 Assigning fd_out, fd_out: %d\n", elements->fd_out);
				elements->fd_in = dup(here_doc(elements->in_f[number_of_infiles].name));
				number_of_infiles++;
			}
			number_of_infiles = 0;
		}
		else
		{
			while(elements->in_f != NULL && elements->in_f[number_of_infiles].name != NULL && elements->in_f[number_of_infiles].type == INPUT_F)
			{
				fprintf(stderr, "Number of assignment 4 Assigning fd_out, fd_out: %d\n", elements->fd_out);
				elements->fd_in = open(elements->in_f[number_of_infiles].name, O_RDONLY);
				fprintf(stderr, "string of files [%d]: %s\n", number_of_infiles, elements->in_f[number_of_infiles].name);
				number_of_infiles++;
			}
			number_of_infiles = 0;
			while(elements->in_f != NULL && elements->in_f[number_of_infiles].type == DELIMITOR)
			{
				fprintf(stderr, "Number of assignment 5 Assigning fd_out, fd_out: %d\n", elements->fd_out);
				elements->fd_in = dup(here_doc(elements->in_f[number_of_infiles].name));
				number_of_infiles++;
			}
			number_of_infiles = 0;
		}
		if (i == vars->num_cmd - 1)
		{
			while(elements->out_f != NULL && elements->out_f[number_of_outfiles].name != NULL && elements->out_f[number_of_outfiles].type == OUTPUT_F)
			{
				fprintf(stderr, "Number of assignment 6 Assigning fd_out, fd_out: %d\n", elements->fd_out);
				elements->fd_out = open(elements->out_f[number_of_outfiles].name, O_WRONLY | O_CREAT | O_TRUNC, 0664);
				number_of_outfiles++;
			}
			number_of_outfiles = 0;
			while(elements->out_f != NULL && elements->out_f[number_of_outfiles].name != NULL && elements->out_f[number_of_outfiles].type == R_AP_OUTPUT_F)
			{
				fprintf(stderr, "Number of assignment 7 Assigning fd_out, fd_out: %d\n", elements->fd_out);
				elements->fd_out = open(elements->out_f[number_of_outfiles].name, O_WRONLY | O_CREAT | O_APPEND, 0664);
				number_of_outfiles++;
			}
			number_of_outfiles = 0;
			// if(elements->indentifier == BUILT_IN_BLOCK)
			// {
			// 	if (strncmp(elements->arguments[0],"echo", strlen("echo")) == 0)
			// 	{
			// 		// elements->fd_out = dup(1);
			// 		fprintf(stderr, "******using echo****** \nfd_out: %d\n first argument: %s\n", STDOUT_FILENO, elements->arguments[1]);
			// 		builtins_echo(elements->fd_out, elements->arguments);
			// 	}
			// }
		}
		else
		{
			/*place for all output except for last one*/
			while(elements->out_f != NULL && elements->out_f[number_of_outfiles].name != NULL && elements->out_f[number_of_outfiles].type == OUTPUT_F)
			{
				fprintf(stderr, "Number of assignment 8 Assigning fd_out, fd_out: %d\n", elements->fd_out);
				elements->fd_out = open(elements->out_f[number_of_outfiles].name, O_WRONLY | O_CREAT | O_TRUNC, 0664);
				number_of_outfiles++;
			}
			number_of_infiles = 0;
			while(elements->out_f != NULL && elements->out_f[number_of_outfiles].name != NULL && elements->out_f[number_of_outfiles].type == R_AP_OUTPUT_F)
			{
				fprintf(stderr, "Number of assignment 9 Assigning fd_out, fd_out: %d\n", elements->fd_out);
				elements->fd_out = open(elements->out_f[number_of_outfiles].name, O_WRONLY | O_CREAT | O_APPEND, 0664);
				number_of_outfiles++;
			}
			number_of_outfiles = 0;
		}
		if(elements->indentifier == BUILT_IN_BLOCK || (elements->indentifier == CMD_BLOCK && elements->command_path != NULL))
		// if(elements->indentifier == CMD_BLOCK && elements->command_path != NULL)
		{
			fprintf(stderr, "Command execution\n");
			pids = fork();
			if (pids == -1)
			{
				freeing_chunks(salt, info);
				write(2, "Error while creating process\n", 30);
			}
			if (pids == 0)
			{
				miner_closing_pipe(pipes, vars, i);
				if (i == 0)
				{
					dup2(elements->fd_in, STDIN_FILENO);
					close(elements->fd_in);
					close(pipes[0][0]);
					close(pipes[0][1]); // this may be interesting
					fprintf(stderr, "Sent data through pipe\n");
				}
				else
				{
					//redirection check on iput in the ones in between
					if(elements->prev != NULL && elements->prev->out_f == NULL && elements->in_f == NULL)
					{
						fprintf(stderr, "Number of assignment 10 Assigning fd_out, fd_out: %d\n", elements->fd_out);
						elements->fd_in = dup(pipes[i][0]);
						// dup2(pipes[i][0], elements->fd_in);
						close(pipes[i][0]);
					}
					dup2(elements->fd_in, STDIN_FILENO);
					close(elements->fd_in);
					fprintf(stderr, "Should recieve data through pipe\n");	
				}
				if (i == vars->num_cmd - 1)
				{
					dup2(elements->fd_out, STDOUT_FILENO);
					close(elements->fd_out);
				}
				else
				{
					if(elements->next != NULL && elements->out_f == NULL && elements->next->in_f == NULL)
					{
						fprintf(stderr, "Number of assignment 11 Assigning fd_out, fd_out: %d\n", elements->fd_out);
						elements->fd_out = dup(pipes[i + 1][1]);
						// dup2(pipes[i + 1][1], elements->fd_out);
						close(pipes[i + 1][1]);
					}
					number_of_outfiles = 0;
					fprintf(stderr, "elements->fd_out %d\n", elements->fd_out);
					dup2(elements->fd_out, STDOUT_FILENO);
					fprintf(stderr, "elements->fd_out after dup 2%d\n", elements->fd_out);
					close(elements->fd_out);
				}
				if(elements->indentifier == CMD_BLOCK)
					run(elements, info, envp);
				else if(elements->indentifier == BUILT_IN_BLOCK)
				{
					// if (strncmp(elements->arguments[0],"echo", strlen("echo")) == 0)
					// {
					// 	// elements->fd_out = dup(1);
					// 	fprintf(stderr, "******using echo****** \nfd_out: %d\n first argument: %s\n", STDOUT_FILENO, elements->arguments[1]);
					// 	builtins_echo(elements->fd_out, elements->arguments);
					// }
					exit(EXIT_SUCCESS);
				}
			}
		}
		elements=elements->next;
		i++;
	}
	
	overseer_closing_pipe(pipes, vars);
	i = 0;
	while(i < vars->num_cmd)
	{
		// If a command is not found, the child process created to execute it 
		// returns a status of 127. If a command is found but is not executable, 
		// the return status is 126.
		waitpid(-1, &status, 0);
		g_exit_status = WEXITSTATUS(status);
		fprintf(stderr,"exit status: %d\n", g_exit_status);
		i++;
		fprintf(stderr, "Parrent waited for process pids[%d]\n", i);
	}
}
	
					
				