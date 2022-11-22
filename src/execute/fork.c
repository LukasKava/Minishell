/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbiederm <pbiederm@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 11:43:52 by pbiederm          #+#    #+#             */
/*   Updated: 2022/11/22 11:41:17 by pbiederm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	second_child(t_chunk **salt, t_info *info, char	**envp)
{
	int		pid;
	int		**fd;
	int		i;
	t_chunk	*local_chunk;

	i = 0;
	fd = (int**)malloc(1000 *sizeof(int*));
	while(i < 2)
	{
		fd[i] = (int*)malloc(1000 * sizeof(int));
		i++;
	}
	local_chunk = *(salt);
	while(local_chunk != NULL)
	{	
		if (local_chunk->indentifier == R_AP_OUTPUT_F)
		{
			fd[0][OUTPUT] = open(local_chunk->arguments[0], O_WRONLY | O_CREAT | O_APPEND, 0664);
		}
		if (local_chunk->indentifier == INPUT_F)
		{
			fd[0][INPUT] = open(local_chunk->arguments[0], O_RDONLY);
		}
		else if (local_chunk->indentifier == OUTPUT_F)
			fd[0][OUTPUT] = open(local_chunk->arguments[0], O_WRONLY | O_CREAT | O_TRUNC, 0664);
		local_chunk = local_chunk->next;
	}
	local_chunk = *(salt);
	pid = fork();
	if (pid < 0)
	{
		freeing_chunks(salt, info);
		printf("Error: while forking single child process.\n");
		return ;
	}
	if (pid == 0)
	{
		roles_expanded(fd, *salt, info, envp);
	}
	while(local_chunk != NULL)
	{	
		if (local_chunk->indentifier== INPUT_F)
			close(fd[0][INPUT]);
		if (local_chunk->indentifier == OUTPUT_F)
			close(fd[0][OUTPUT]);
		if (local_chunk->indentifier == R_AP_OUTPUT_F)
			close(fd[0][OUTPUT]);
		local_chunk = local_chunk->next;
	}
	free_fd(fd);
	waitpid(pid, NULL, 0);
}

typedef struct s_vars
{
	int		pipes[9999][2];
	int		num_pipes;
	int		exec_fd[2];
	t_chunk	*run_chunk;
}t_vars;

void	third_child(t_chunk **salt, t_info *info, char	**envp)
{
	int		i;
	int		j;
	int		s;
	int		num_pipes;
	int 	pids[9999];
	int		pipes[9999][2];
	int		**fd;
	t_chunk	*local_chunk;
	t_vars	vars;

	j = 0;
	fd = (int**)malloc(1000 * sizeof(int*));
	while(j < 2)
	{
		fd[j] = (int*)malloc(1 * sizeof(int));
		j++;
	}
	// fd[j] = NULL;
	num_pipes = 0;
	local_chunk = *salt;
	while(local_chunk != NULL)
	{
		if (local_chunk->indentifier == CMD_BLOCK)
		{
			if (pipe(pipes[num_pipes]) == -1) 
			{
				printf("Error with creating pipe\n");
				return ;
			}
			printf("Created pipe[%d] \n", num_pipes);
			num_pipes++;
		}
		local_chunk = local_chunk->next;
	}
	vars.num_pipes = num_pipes;
	// while (s < 1000)
	// {
	// 	fd[s][OUTPUT] = 1;
	// 	fd[s][INPUT] = 0;
	// }
	s = 0;
	local_chunk = *salt;
	while(local_chunk != NULL)
	{	
		if (local_chunk->indentifier == R_AP_OUTPUT_F)
		{
			fd[0][OUTPUT] = open(local_chunk->arguments[0], O_WRONLY | O_CREAT | O_APPEND, 0664);
		}
		else if (local_chunk->indentifier == INPUT_F)
		{
			fd[0][INPUT] = open(local_chunk->arguments[0], O_RDONLY);
		}
		else if (local_chunk->indentifier == OUTPUT_F)
		{
			fd[0][OUTPUT] = open(local_chunk->arguments[0], O_WRONLY | O_CREAT | O_TRUNC, 0664);
			// fprintf(stderr, "the s equals %d", s);
			// fd[1][OUTPUT] = 33;
		}
		else if (local_chunk->indentifier == CMD_BLOCK)
		{
			s++;
		}

		fprintf(stderr, "S value equals %d\n", s);
		local_chunk = local_chunk->next;
	}
	local_chunk = *salt;
	i = -1;
	while (local_chunk != NULL)
	{
		if (local_chunk->indentifier == CMD_BLOCK)
		{
			i++;
			vars.run_chunk = local_chunk;
			pids[i] = fork();
			if (pids[i] == -1)
			{
				printf("Error with creating process\n");
				return ;
			}
			if (pids[i] == 0)
			{
				//closing all efdees of pipes
				num_pipes = 0;
				while(num_pipes < vars.num_pipes)
				{
					if(num_pipes != i)
						close(pipes[num_pipes][0]);
					if(i + 1 != num_pipes)
						close(pipes[num_pipes][1]);
					num_pipes++;
				}
				// fprintf(stderr, "I am child of number %d the Fd[1][OUT]: %d\n", i, fd[1][OUTPUT]);

				if (i == 0)
				{
					// dup2(fd[0][INPUT], STDIN_FILENO);
					// close(fd[0][INPUT]);
					close(pipes[0][0]);
					close(pipes[1][0]);
				}
				else
				{
					// dup2(pipes[i][0], STDIN_FILENO);
					// close(pipes[i][0]);
					dup2(pipes[i][0], STDIN_FILENO);
					close(pipes[i][0]);
				}
				fprintf (stderr, "num pipes %d\n", vars.num_pipes);
				if (i == vars.num_pipes - 1)
				{
					fprintf(stderr, "the i equals %d\n",i );
					// dup2(fd[i+1][OUTPUT], STDOUT_FILENO);
					// close(fd[i+1][OUTPUT]);
					// dup2(fd[0][OUTPUT], STDOUT_FILENO);
					// close(fd[0][OUTPUT]);
					close(pipes[1][1]);
				}
				else
				{
					dup2(pipes[i + 1][1], STDOUT_FILENO);
					close(pipes[i + 1][1]);
				}
				run(vars.run_chunk, info, envp);
			}
		}
		local_chunk = local_chunk->next;
	}
	fprintf(stderr, "Parent process here \n");
	num_pipes = vars.num_pipes;
	while(num_pipes)
	{
			num_pipes--;
			close(pipes[num_pipes][0]);
			close(pipes[num_pipes][1]);
	}
	num_pipes = vars.num_pipes;
	while(num_pipes > 0)
	{
		wait(NULL);
		num_pipes--;
	}
}