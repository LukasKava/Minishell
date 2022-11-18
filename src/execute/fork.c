/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbiederm <pbiederm@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 11:43:52 by pbiederm          #+#    #+#             */
/*   Updated: 2022/11/18 14:08:25 by pbiederm         ###   ########.fr       */
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
	int	num_pipes;
}t_vars;

void	third_child(t_chunk **salt, t_info *info, char	**envp)
{
	int		i;
	int		num_pipes;
	int 	pids[9999];
	int		pipes[9999][2];
	// int		fd[2];
	t_chunk	*local_chunk;
	t_vars	vars;

	// fd[0] = open("unbroken_circle.txt", O_RDONLY);
	// fd[1] = open("copy_dog.txt", O_CREAT | O_RDWR | O_TRUNC, 0644);
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
			num_pipes++;
		}
		local_chunk = local_chunk->next;
	}
	vars.num_pipes = num_pipes;
	local_chunk = *salt;
	// if (pipe(pipes[num_pipes]) == -1) 
	// {
	// 	printf("Error with creating pipe\n");
	// 	return 1;
	// }
	i = -1;
	local_chunk = *salt;
	while (local_chunk != NULL)
	{	
		if (local_chunk->indentifier == CMD_BLOCK)
		{
			i++;
			pids[i] = fork();
			if (pids[i] == -1)
			{
				printf("Error with creating process\n");
				return ;
			}
			if (pids[i] == 0)
			{
				//closing all efdees of pipes
				while(num_pipes--)
				{
					if(num_pipes != i)
						close(pipes[num_pipes][0]);
					if(num_pipes + 1 != i)
						close(pipes[num_pipes][1]);
				}
				num_pipes = vars.num_pipes;
				if(pids[i] == 0)
				{
					fprintf(stderr, "I am child of number %d\n", i);
					//file des configuration here
					if (i == 0)
					// {
						close(pipes[i][0]);
					// 	dup2(fd[0], STDIN_FILENO);
					// 	close(fd[0]);
					}
					else
					{
						// close(fd[0]);
						dup2(pipes[i][0], STDIN_FILENO);
						close(pipes[i][0]);
					}
					fprintf (stderr, "num pipes %d\n", num_pipes);
					if (i == num_pipes - 1)
					{
						close(pipes[i + 1][1]);
						// dup2(fd[1], STDOUT_FILENO);
						// close(fd[1]);
					}
					else
					{
						// close(fd[1]);
						dup2(pipes[i + 1][1], STDOUT_FILENO);
						close(pipes[i + 1][1]);
					}
				run(local_chunk, info, envp);
			}
		}	
		local_chunk = local_chunk->next;
	}
	fprintf(stderr, "Parent process here \n");
	while(num_pipes--)
	{
			close(pipes[num_pipes][0]);
			close(pipes[num_pipes][1]);
			// num_pipes--;
	}
	while(i>=0)
	{
		wait(NULL);
		i--;
	}
}