/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbiederm <pbiederm@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 11:43:52 by pbiederm          #+#    #+#             */
/*   Updated: 2022/11/10 15:18:59 by pbiederm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	input_first(int **fd, t_chunk	*salt, t_info *info, char	**envp)
{
	t_chunk	*local_chunk;

	local_chunk = *(&salt);
	dup2(fd[0][INPUT], STDIN_FILENO);
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
	close(fd[0][INPUT]);
	free_fd(fd);
	run(salt, info, envp);
}


void	output_first(int **fd, t_chunk	*salt, t_info *info, char	**envp)
{
	t_chunk	*local_chunk;

	local_chunk = *(&salt);
	dup2(fd[0][OUTPUT], STDOUT_FILENO);
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
	close(fd[0][OUTPUT]);
	free_fd(fd);
	run(salt, info, envp);
}

void	input_output(int **fd, t_chunk	*salt, t_info *info, char	**envp)
{
	t_chunk	*local_chunk;

	local_chunk = &(*salt);
	dup2(fd[0][INPUT], STDIN_FILENO);
	dup2(fd[0][OUTPUT], STDOUT_FILENO);
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
	run(salt, info, envp);
}

void	roles_expanded(int **fd, t_chunk	*salt, t_info *info, char	**envp)
{
	
	if (salt->indentifier == CMD_BLOCK && salt->next == NULL)
		run(salt, info, envp);
	else if (salt->indentifier == CMD_BLOCK && salt->next->indentifier == INPUT_F && salt->next->next == NULL)
		input_first(fd, salt, info, envp);
	else if (salt->indentifier == CMD_BLOCK && (salt->indentifier == R_AP_OUTPUT_F||salt->next->indentifier == OUTPUT_F) && salt->next->next == NULL)
		output_first(fd, salt, info, envp);
	else
		input_output(fd, salt, info, envp);
}

void	free_fd(int **fd)
{
	int	i;

	i = 0;
	while(i < 2)
	{
		free(fd[i]);
		i++;
	}
	free(fd);
}

void	second_child(t_chunk	**salt, t_info *info, char	**envp)
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
		// roles_expanded(fd, *salt, info, envp);
		here_doc(salt, info, envp);
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

// either write to a file, use get next line, either use a pipe(write to a pipe, read from a pipe), remove the file
void	here_doc(t_chunk	**salt, t_info *info, char	**envp)
{
	t_chunk	*local_chunk;
	char	*delimit;
	int		pfd[2];
	char*	buff;

	buff = malloc(9999);
	pipe(pfd);
	local_chunk = *salt;
	while(local_chunk != NULL)
	{	
		if (local_chunk->indentifier == DELIMITOR)
			break ;
		local_chunk = local_chunk->next;
	}
	delimit = local_chunk->arguments[0];
		while (TRUE)
		{
			write(2, "> ", 2);
			buff = get_next_line(STDIN_FILENO);
			if (ft_strncmp(buff, delimit, ft_strlen(delimit)) == 0)
				break;
			write(pfd[OUTPUT], buff, strlen(buff));
		}
		close(pfd[OUTPUT]);
		dup2(pfd[INPUT], STDIN_FILENO);
		close(pfd[INPUT]);
		free(buff);
		run(*salt, info, envp);
}


