/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbiederm <pbiederm@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 11:43:52 by pbiederm          #+#    #+#             */
/*   Updated: 2022/11/07 17:15:00 by pbiederm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	input_first(int *fd, t_chunk	*salt, t_info *info, char	**envp)
{
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
	free(fd);
	run(salt, info, envp);
}

void	output_first(int *fd, t_chunk	*salt, t_info *info, char	**envp)
{
	dup2(fd[1], STDOUT_FILENO);
	close(fd[1]);
	free(fd);
	run(salt, info, envp);
}

void	roles(int *fd, t_chunk	*salt, t_info *info, char	**envp)
{
	if (salt->indentifier == CMD_BLOCK && salt->next == NULL)
		run(salt, info, envp);
	if (salt->indentifier == CMD_BLOCK && salt->next->indentifier == INPUT_F)
		input_first(fd, salt, info, envp);
	if (salt->indentifier == CMD_BLOCK && salt->next->indentifier == OUTPUT_F)
		output_first(fd, salt, info, envp);
}

void	first_child(t_chunk	*salt, t_info *info, char	**envp)
{
	int		pid;
	int		*fd;

	fd = (int*)malloc(2 * sizeof(int));

	if (salt->next != NULL)
	{
		if (salt->next->indentifier == INPUT_F)
			{
				fd[0] = open(salt->next->arguments[0], O_RDONLY);
				if(fd[0] < 0)
					printf("Error: no such file.\n");
					free(fd);
					return ;
			}
		
		if (salt->next->indentifier == OUTPUT_F)
			fd[1] = open(salt->next->arguments[0], O_WRONLY | O_CREAT | O_TRUNC, 0664);
	}
	pid = fork();
	if (pid < 0)
	{
		freeing_chunks(&salt, info);
		printf("Error: while forking single child process.\n");
		return ;
	}
	if (pid == 0)
	{
		roles(fd, salt, info, envp);
	}
	if (salt->next != NULL)
	{
		if (salt->next->indentifier == INPUT_F)
			close(fd[0]);
		if (salt->next->indentifier == OUTPUT_F)
			close(fd[1]);
		free(fd);
	}
	waitpid(pid, NULL, 0);
}

void	input_output(int **fd, t_chunk	*salt, t_info *info, char	**envp)
{
	dup2(fd[0][INPUT], STDIN_FILENO);
	close(fd[0][INPUT]);
	dup2(fd[0][OUTPUT], STDOUT_FILENO);
	close(fd[0][OUTPUT]);
	free_fd(fd);
	run(salt, info, envp);
}

void	roles_expanded(int **fd, t_chunk	*salt, t_info *info, char	**envp)
{
	if (salt->indentifier == CMD_BLOCK && salt->next->indentifier == INPUT_F && salt->next->next->indentifier == OUTPUT_F)
		input_output(fd, salt, info, envp);
}

void	free_fd(int **fd)
{
	int	i;

	i = 0;
	while(i < 3)
	{
		free(fd[i]);
		i++;
	}
	free(fd);
}

void	second_child(t_chunk	*salt, t_info *info, char	**envp)
{
	int	pid;
	int	**fd;
	int i;

	i = 0;
	fd = (int**)malloc(1 *sizeof(int*));
	while(i < 3)
	{
		fd[i] = (int*)malloc(2 * sizeof(int));
		i++;
	}
	//infile
	fd[0][INPUT] = open(salt->next->arguments[0], O_RDONLY);
	fd[0][OUTPUT] = open(salt->next->next->arguments[0], O_WRONLY | O_CREAT | O_TRUNC, 0664);
	pid = fork();
	if (pid < 0)
	{
		freeing_chunks(&salt, info);
		printf("Error: while forking single child process.\n");
		return ;
	}
	if (pid == 0)
	{
		roles_expanded(fd, salt, info, envp);
	}
	close(fd[0][INPUT]);
	close(fd[0][OUTPUT]);
	free_fd(fd);
	waitpid(pid, NULL, 0);
}


