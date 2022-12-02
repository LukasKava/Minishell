/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_fork.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbiederm <pbiederm@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/27 11:52:18 by pbiederm          #+#    #+#             */
/*   Updated: 2022/12/02 17:20:14 by pbiederm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"../../includes/minishell.h"

int	**create_pipes(t_chunk **salt, t_vars	*vars)
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
	int		read_fd;
	int		write_fd;
	int		number_of_infiles;
	int		number_of_outfiles;
	// int		number_of_pipes;

	i = 0;
	number_of_infiles = 0;
	number_of_outfiles = 0;
	// number_of_pipes = 0;
	read_fd = dup(0);
	write_fd = dup(1);
	vars = malloc(sizeof(*vars));
	elements = *salt;
	pipes = create_pipes(salt, vars);
	fprintf(stderr,"vars.num_cmd = %d\n", vars->num_cmd);
	while(elements)
	{
		//or redirections here
		if (i == 0)
		{
			while(elements->in_f != NULL && elements->in_f[number_of_infiles].name != NULL)
			{
				read_fd = open(elements->in_f[number_of_infiles].name, O_RDONLY);
				fprintf(stderr, "string of files [%d]: %s\n", number_of_infiles, elements->in_f[number_of_infiles].name);
				number_of_infiles++;
			}
			number_of_infiles = 0;
		}
		else
		{
			// redirection check oni put in the ones in between
			while(elements->in_f != NULL && elements->in_f[number_of_infiles].name != NULL)
			{
				pipes[i][0] = open(elements->in_f[number_of_infiles].name, O_RDONLY);
				fprintf(stderr, "string of files [%d]: %s\n", number_of_infiles, elements->in_f[number_of_infiles].name);
				number_of_infiles++;
			}
			number_of_infiles = 0;
		// 	fprintf(stderr, "Should recieve data through pipe\n");
		}
		if (i == vars->num_cmd - 1)
		{
			while(elements->out_f != NULL && elements->out_f[number_of_outfiles].name != NULL)
			{
				write_fd = open(elements->out_f[number_of_outfiles].name, O_WRONLY | O_CREAT | O_TRUNC, 0664);
				number_of_outfiles++;
			}
			number_of_outfiles = 0;
		}
		if(elements->indentifier == CMD_BLOCK)
		{
			pids = fork();
			if (pids == 0)
			{
				miner_closing_pipe(pipes, vars, i);
				//maybe redirections hre
				
				if (i == 0)
				{
					dup2(read_fd, STDIN_FILENO);
					close(read_fd);
					close(pipes[0][0]);
					fprintf(stderr, "Sent data through pipe\n");
				}
				else
				{
					//redirection check on iput in the ones in between
					dup2(pipes[i][0], STDIN_FILENO);
					close(pipes[i][0]);
					fprintf(stderr, "Should recieve data through pipe\n");	
				}
				if (i == vars->num_cmd - 1)
				{
					// redirection check in the last one
					dup2(write_fd, STDOUT_FILENO);
					close(write_fd);
				}
				else
				{
					while(elements->out_f != NULL && elements->out_f[number_of_outfiles].name != NULL)
					{
						pipes[i + 1][1] = open(elements->out_f[number_of_outfiles].name, O_WRONLY | O_CREAT | O_TRUNC, 0664);
						number_of_outfiles++;
					}
					number_of_outfiles = 0;
					dup2(pipes[i + 1][1], STDOUT_FILENO);
					close(pipes[i + 1][1]);
					fprintf(stderr, "Sent data through pipe\n");
				}
				// while(pipes[number_of_pipes])
				// {
				// 	free(pipes[number_of_pipes]);
				// 	number_of_pipes++;
				// }
				// free(pipes);
				run(elements, info, envp);
			}
			elements=elements->next;
			i++;
		}
	}
	overseer_closing_pipe(pipes, vars);
	// while(pipes[number_of_pipes])
	// {
	// 	fprintf(stderr,"number of pipes %d", pipes[number_of_pipes]);
	// 	free(pipes[number_of_pipes]);
	// 	number_of_pipes++;
	// }
	// free(pipes);
	i = 0;
	while(i < vars->num_cmd)
	{
		waitpid(-1, NULL, 0);
		i++;
		fprintf(stderr, "Parrent waited for process pids[%d]\n", i);
	}
}
					
				