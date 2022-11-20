/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbiederm <pbiederm@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 11:43:52 by pbiederm          #+#    #+#             */
/*   Updated: 2022/11/20 16:04:55 by pbiederm         ###   ########.fr       */
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
	int		num_pipes;
	int		fd[2];
	t_chunk	*run_chunk;
}t_vars;

void	third_child(t_chunk **salt, t_info *info, char	**envp)
{
	int		i;
	int		j;
	int		num_pipes;
	int 	pids[9999];
	int		pipes[9999][2];
	int		**fd;
	t_chunk	*local_chunk;
	t_vars	vars;

	// fd[0] = open("unbroken_circle.txt", O_RDONLY);
	// fd[1] = open("copy_dog.txt", O_CREAT | O_RDWR | O_TRUNC, 0644);
	j = 0;
	fd = (int**)malloc(1000 * sizeof(int*));
	while(j < 2)
	{
		fd[j] = (int*)malloc(2 * sizeof(int));
		j++;
	}
	// fd[j] = NULL;
	local_chunk = *(salt);
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
			fd[0][OUTPUT] = open(local_chunk->arguments[0], O_WRONLY | O_CREAT | O_TRUNC, 0664);
		local_chunk = local_chunk->next;
	}
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
	local_chunk = *salt;
	i = -1;
	while (local_chunk != NULL)
	{	
		if (local_chunk->indentifier == CMD_BLOCK)
		{
			i++;
			vars.run_chunk = local_chunk;
			// if (find_red == NULL)
			// 	break ;
			// // here I an traverse the nodes in search for the redirections
			// return ;
			// local_chunk = vars.run_chunk;
			// fprintf(stderr, "after rewriting local chunk id: %d\n", vars.run_chunk->indentifier);
			// vars.fd[0] = 15;
			// vars.fd[1] = 16;
			// if (i == 1)
			// {
			// 	vars.fd[0]++;
			// 	vars.fd[1]++;
			// }
			// fprintf(stderr, "local chunk identifier %d\n", vars.run_chunk->indentifier);
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
				// fprintf(stderr, "I am child of number %d fd[1] = %d, fd[0] = %d\n", i, vars.fd[0], vars.fd[1]);
				fprintf(stderr, "I am child of number %d", i);
				//file des configuration here
				// traverse and see what other redirections are here
				// but do so in a diff
				// Below is input side
				if (i == 0)
				{
					dup2(fd[0][INPUT], STDIN_FILENO);
					close(fd[0][INPUT]);
					close(pipes[0][0]);
					close(pipes[1][0]);
				}
				else
				{
					dup2(pipes[i][0], STDIN_FILENO);
					close(pipes[i][0]);
				}
				fprintf (stderr, "num pipes %d\n", vars.num_pipes);
				if (i == vars.num_pipes - 1)
				{
					dup2(fd[0][OUTPUT], STDOUT_FILENO);
					close(fd[0][OUTPUT]);
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