/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbiederm <pbiederm@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 11:43:52 by pbiederm          #+#    #+#             */
/*   Updated: 2022/11/26 14:07:40 by pbiederm         ###   ########.fr       */
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




void	third_child(t_chunk **salt, t_info *info, char	**envp)
{
	int		i;
	int		s;
	int		num_pipes;
	int 	pids[9999];
	int		fd[9999][2];
	t_chunk	*local_chunk;
	t_vars	vars;

	num_pipes = 1;
	local_chunk = *salt;
	while(local_chunk != NULL)
	{
		if (local_chunk->indentifier == CMD_BLOCK && local_chunk->next->indentifier == CMD_BLOCK)
		{
			if (pipe(fd[num_pipes]) == -1) 
			{
				printf("Error with creating pipe\n");
				return ;
			}
		}
		num_pipes++;
		local_chunk = local_chunk->next;
	}
	vars.num_pipes = num_pipes;
	s = 0;
	local_chunk = *salt;
	local_chunk = local_chunk->next;
	while(local_chunk != NULL)
	{	
		if (local_chunk->indentifier == R_AP_OUTPUT_F)
		{
			fd[s][OUTPUT] = open(local_chunk->arguments[0], O_WRONLY | O_CREAT | O_APPEND, 0664);
		}
		else if (local_chunk->indentifier == INPUT_F)
		{
			fd[s][INPUT] = open(local_chunk->arguments[0], O_RDONLY);
		}
		else if (local_chunk->indentifier == OUTPUT_F)
		{
			fprintf(stderr, "the s %d", s);
			fd[s][OUTPUT] = open(local_chunk->arguments[0], O_WRONLY | O_CREAT | O_TRUNC, 0664);
		}
		else if (local_chunk->indentifier == CMD_BLOCK)
		{
			s++;
		}
		local_chunk = local_chunk->next;
	}
	local_chunk = *salt;
	i = -1;
	while (local_chunk != NULL)
	{
		if (local_chunk->indentifier == CMD_BLOCK)
		{
			i++;
			// all redirections need to be done here
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
						close(fd[num_pipes][0]);
					if(i + 1 != num_pipes)
						close(fd[num_pipes][1]);
					num_pipes++;
				}
				// fprintf(stderr, "I am child of number %d the Fd[1][OUT]: %d\n", i, fd[1][OUTPUT]);

				if (i == 0)
				{
				}
				else
				{
					// stdin(0) cmd 1 == 1 cmd 0 == 1 7 cmd 0 == 1 cmd stdout(0)
					dup2(fd[i][INPUT], STDIN_FILENO);
					close(fd[i][INPUT]);
					// dup2(pipes[i][0], STDIN_FILENO);
					// close(pipes[i][0]);
				}
				// fprintf (stderr, "num pipes %d\n", vars.num_pipes);
				if (i == vars.num_pipes - 1)
				{
					fprintf(stderr, "the i equals %d\n",i );
					// dup2(fd[2][OUTPUT], STDOUT_FILENO);
					// close(fd[2][OUTPUT]);
					// dup2(fd[i + 1][OUTPUT], STDOUT_FILENO);
					close(fd[i + 1][OUTPUT]);
					// close(pipes[1][1]);
				}
				else
				{
					dup2(fd[i + 1][OUTPUT], STDOUT_FILENO);
					close(fd[i + 1][OUTPUT]);
					// dup2(pipes[i + 1][1], STDOUT_FILENO);
					// close(pipes[i + 1][1]);
				}
				run(vars.run_chunk, info, envp);
			}
		}
		local_chunk = local_chunk->next;
	}
	fprintf(stderr, "Parent process here \n");
	num_pipes = vars.num_pipes;
	// while(num_pipes)
	// {
	// 		num_pipes--;
	// 		close(pipes[num_pipes][0]);
	// 		close(pipes[num_pipes][1]);
	// }
	// 	while(num_pipes)
	// {
	// 		num_pipes--;
	// 		close(fd[num_pipes][0]);
	// 		close(fd[num_pipes][1]);
	// }
	num_pipes = vars.num_pipes;
	while(num_pipes > 0)
	{
		wait(NULL);
		num_pipes--;
	}
}

int is_next_red_in(t_chunk	**salt);
int is_red_out(t_chunk	**salt);
int	is_prev_red_out(t_chunk **salt);
int is_red_in(t_chunk **salt);

void	fourth_child(t_chunk **salt, t_info *info, char **envp)
{
	t_chunk	*elements;
	int 	pids[9999];
	int		pipe_fd[2];
	int		i;

	elements = *salt;
	// fprintf(stderr,"Is this one out? %d\n", is_red_out(&elements));
	// fprintf(stderr,"Is next one in? %d\n", is_next_red_in(&elements));
	// fprintf(stderr,"Is this one in? %d\n", is_red_in(&elements));
	// fprintf(stderr,"Is prev one in? %d\n", is_prev_red_out(&elements));
	// return ;
	i = 0;
	while(elements)
	{
		if (!(is_next_red_in(&elements)) && !(is_red_out(&elements)))
		// if(i == 0)
		{
			if(pipe(pipe_fd) == -1)
			{
				write(2, "pipe Error\n", 12);
			}
			// elements->next->exec_fd[0] = dup(pipe_fd[0]);
			// elements->exec_fd[1] = dup(pipe_fd[1]);
		}
		if(elements->indentifier == CMD_BLOCK)
		{
			pids[i] = fork();
			if(pids[i] == 0)
			{
				if ((!is_next_red_in(&elements)) && !(is_red_out(&elements)))
				// if(i == 1)
				{
					close(pipe_fd[1]);
					dup2(pipe_fd[0], STDIN_FILENO);
					close(pipe_fd[0]);
				}
				// else if (!(is_prev_red_out(&elements)) && !(is_red_in(&elements)))
				if (i == 0)
				{
					close(pipe_fd[0]);
					dup2(pipe_fd[1], STDOUT_FILENO);
					close(pipe_fd[1]);
					// fprintf(stderr,"Is this one out? %d\n", is_red_out(&elements));
				}
					run(elements, info, envp);
			}
			i++;
		}
		elements = elements->next;
	}
		elements = *salt;
		// close(elements->exec_fd[0]);
		// close(elements->exec_fd[1]);
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		wait(NULL);
		wait(NULL);
}

// A function that checks the in redirections. Works only if the first one is the command block.
int is_next_red_in(t_chunk	**salt)
{
	t_chunk *element;

	element = *salt;
	if(element != NULL)
		element = element->next;
	while(element->indentifier != CMD_BLOCK && element != NULL)
	{
		element = element->next;
	}
	printf("Somethin\n");
	if(element != NULL)
	{
		printf("Somethin else\n");
		element = element->next;
	}
	while(element != NULL) 
	{
		if(element->indentifier == CMD_BLOCK)
			return (0);
		if(element->indentifier == INPUT_F)
			return (1);
		element = element->next;
	}
	return (0);
}

// Checks redirections out. It can be usefull when deciding if to pipe.
int is_red_out(t_chunk	**salt)
{
	t_chunk *element;

	element = *salt;
	if(element != NULL)
		element = element->next;
	while(element != NULL)
	{
		if(element->indentifier == CMD_BLOCK)
			return(0);
		if(element->indentifier == OUTPUT_F)
			return (1);
		if (element->next == NULL)
		{
			printf("Spam \n");
			break;
		}
		element = element->next;
	}
	fprintf(stderr, "seg check\n");
	exit (0);
	return (0);
}

int	is_prev_red_out(t_chunk **salt)
{
	t_chunk *element;

	element = *salt;
	if (element != NULL)
		element = element->prev;
	while(element != NULL)
	{
		if(element->indentifier == CMD_BLOCK)
			return(0);
		if(element->indentifier == OUTPUT_F)
			return(1);
		element = element->prev;
	}
	return(0);
}

int is_red_in(t_chunk **salt)
{
	t_chunk *element;
	
	element = *salt;
	if (element != NULL)
		element = element->next;
	while (element)
	{
		if(element->indentifier == CMD_BLOCK)
			return (0);
		if(element->indentifier == INPUT_F)
			return(1);
		element=element->next;
	}
	return(0);
}

// translate it