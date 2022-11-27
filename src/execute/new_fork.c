/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_fork.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbiederm <pbiederm@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/27 11:52:18 by pbiederm          #+#    #+#             */
/*   Updated: 2022/11/27 22:45:04 by pbiederm         ###   ########.fr       */
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
		ret_table[num_pipes] = malloc(2 * sizeof(int));
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

int	is_output_redirection(t_chunk **salt, t_vars *vars)
{
	t_chunk	*elements;
	int		write_fd;
	// int		read_fd;
	int		flag;

	flag = 0;
	elements = *salt;
	if(elements && elements->indentifier == CMD_BLOCK)
		elements = elements->next;
	while(elements)
	{
		if (elements->indentifier == OUTPUT_F)
		{
			write_fd = open(elements->arguments[0], O_WRONLY | O_CREAT | O_TRUNC, 0664);
			flag = 1;
		}
		if (elements->indentifier == CMD_BLOCK)
			break;
		elements = elements->next;
	}
	vars->write_fd = write_fd;
	return(flag);
	
}
void	execute(t_chunk **salt, t_info *info, char	**envp)
{
	t_chunk	*elements;
	t_vars	*vars;
	int		**pipes;
	int		pids;
	int		i;
	// int		write_fd;
	// int		read_fd;

	// read_fd = -1;
	i = 0;
	vars = malloc(sizeof(vars));
	elements = *salt;
	pipes = create_pipes(salt, vars);
	fprintf(stderr,"vars.num_cmd = %d\n", vars->num_cmd);
	while(elements)
	{
		//or redirections here
		if(elements->indentifier == CMD_BLOCK)
		{
			// write_fd = dup(pipes[i + 1][1]);
			// close(pipes[i + 1][1]);
			// read_fd = dup(pipes[i][0]);
			// close(pipes[i][0]);
			// redirections here
			pids = fork();
			if (pids == 0)
			{
				miner_closing_pipe(pipes, vars, i);
				//maybe redirections hre
				
				if (i == 0)
				{
					//redirecton chceck in first one
					// read_fd = open("boop.ttxt", O_RDONLY);
					dup2(read_fd, STDIN_FILENO);
					close(read_fd);
					fprintf(stderr, "Sent data through pipe\n");
				}
				else
				{
					// dup2(read_fd, STDIN_FILENO);
					// close(read_fd);
					// ls | cat < boop.ttxt
					//redirection check on iput in the ones in between
					// pipes[i][0] = open("boop.ttxt", O_RDONLY);
					dup2(pipes[i][0], STDIN_FILENO);
					close(pipes[i][0]);
					fprintf(stderr, "Should recieve data through pipe\n");	
				}
				if (i == vars->num_cmd - 1)
				{
					// redirection check in the last one
					// pipes[i][0] = open("boop.ttxt", O_RDONLY);
					// write_fd = open("ttestcase.txt", O_WRONLY | O_CREAT | O_TRUNC, 0664);
					// dup2(write_fd, STDOUT_FILENO);
					// close(write_fd);
					dup2(pipes[i][0], STDIN_FILENO);
					close(pipes[i][0]);
					// dup2(read_fd, STDIN_FILENO);
					// close(read_fd);
					fprintf(stderr, "Should recieve data through pipe\n");
				}
				else
				{
		//stin (0) cat |not 1 but pipe_fd (3) not 0 but pipe_fd(4) lstdout(1)
					// dup2(write_fd, STDOUT_FILENO);
					// close(write_fd);
					//redirection check in the ones in between for the output
					dup2(pipes[i + 1][1], STDOUT_FILENO);
					close(pipes[i + 1][1]);
					// dup2(0, STDOUT_FILENO)
					fprintf(stderr, "Sent data through pipe\n");
				}
				run(elements, info, envp);
			}
			elements=elements->next;
			i++;
		}
	}
	overseer_closing_pipe(pipes, vars);
	// close(write_fd);
	// close(read_fd);
	i = 0;
	while(i < vars->num_cmd)
	{
		waitpid(-1, NULL, 0);
		i++;
		fprintf(stderr, "Parrent waited for process pids[%d]\n", i);
	}
}
