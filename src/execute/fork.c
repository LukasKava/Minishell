/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbiederm <pbiederm@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 11:43:52 by pbiederm          #+#    #+#             */
/*   Updated: 2022/11/07 16:30:54 by pbiederm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// void	exec_handle(t_chunk	*salt, t_info *info, char	**envp)
// {
	// switch between the chunks
// }

// void	from_infile(t_chunk	*salt, t_info *info, char	**envp)
// {
// 	int	infile;
// 	int	pid;

// 	// printf("salt->next->arguments : %s", *salt->next->arguments);
// 	// return ;
// 	infile = open(*(salt->next->arguments), O_RDONLY);
// 	pid = fork();
// 	if (pid < 0)
// 	{
// 		freeing_chunks(&salt, info);
// 		printf("Error: while making fork.\n");
// 	}
// 	if (pid == 0)
// 	{
// 		dup2(infile, STDIN_FILENO);
// 		close(infile);
// 		run(salt, info, envp);
// 	}
// 	waitpid(pid, NULL, 0);
// }

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

void	input_first_exp(int **fd, t_chunk	*salt, t_info *info, char	**envp)
{
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
	free(fd);
	run(salt, info, envp);
}

void	output_first_exp(int **fd, t_chunk	*salt, t_info *info, char	**envp)
{
	dup2(fd[1], STDOUT_FILENO);
	close(fd[1]);
	free_fd(fd)
	run(salt, info, envp);
}

void	roles_expanded(int **fd, t_chunk	*salt, t_info *info, char	**envp)
{
	if (salt->indentifier == CMD_BLOCK && salt->next->indentifier == INPUT_F)
		input_first(fd, salt, info, envp);
}

void	free_fd(int **fd)
{
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
	int	second_pid;
	int	**fd;
	int i;

	i = 0;
	fd = (int**)malloc(2 *sizeof(int*));
	while(i < 3)
	{
		fd[i] = (int*)malloc(2 * sizeof(int));
		i++;
	}
	//infile
	fd[0][LEFT] = open(salt->next->arguments[0], O_RDONLY);
	fd[0][RIGHT] = open(salt->next->arguments[0], O_WRONLY | O_CREAT | O_TRUNC, 0664);
	fd[1][LEFT] = open(salt->next->next->arguments[0], O_RDONLY);
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
	second_pid = fork();
	if (second_pid < 0)
	{
		freeing_chunks(&salt, info);
		printf("Error: while forking single child process.\n");
		return ;
	}
	if (second_pid == 0)
	{
		roles_expanded(fd, salt, info, envp);
	}
	free_fd(fd);
	waitpid(pid, NULL, 0);
}


