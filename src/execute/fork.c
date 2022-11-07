/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbiederm <pbiederm@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 11:43:52 by pbiederm          #+#    #+#             */
/*   Updated: 2022/11/07 14:14:10 by pbiederm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// void	exec_handle(t_chunk	*salt, t_info *info, char	**envp)
// {
	// switch between the chunks
// }

void	from_infile(t_chunk	*salt, t_info *info, char	**envp)
{
	int	infile;
	int	pid;

	// printf("salt->next->arguments : %s", *salt->next->arguments);
	// return ;
	infile = open(*(salt->next->arguments), O_RDONLY);
	pid = fork();
	if (pid < 0)
	{
		freeing_chunks(&salt, info);
		printf("Error: while making fork.\n");
	}
	if (pid == 0)
	{
		dup2(infile, STDIN_FILENO);
		close(infile);
		run(salt, info, envp);
	}
	waitpid(pid, NULL, 0);
}

void	single_child(t_chunk	*salt, t_info *info, char	**envp)
{
	int		pid;

	pid = fork();
	if (pid < 0)
	{
		freeing_chunks(&salt, info);
		// printf("terst: %d\n", info->d_quotes);
		printf("Error: while forking single child process.\n");
		return ;
	}
	if (pid == 0)
	{
		run(salt, info, envp);
	}
	waitpid(pid, NULL, 0);
}

void	roles(int *fd, t_chunk	*salt, t_info *info, char	**envp)
{
	if (salt->indentifier == CMD_BLOCK && salt->next == NULL)
	{
		// close(fd[0]);
		// close(fd[1]);
		run(salt, info, envp);
		return ;
	}
	if (salt->indentifier == CMD_BLOCK && salt->next->indentifier == INPUT_F)
	{
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		close(fd[1]);
		run(salt, info, envp);
		return ;
	}
	if (salt->indentifier == CMD_BLOCK && salt->next->indentifier == OUTPUT_F)
	{
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		close(fd[0]);
		run(salt, info, envp);
		return ;
	}
}

void	first_child(t_chunk	*salt, t_info *info, char	**envp)
{
	int		pid;
	int		fd[2];
	int 	i;

	i = 0;
	// while (salt->next != NULL)
	// {
	// 	printf("argument %s \n", salt->command_path);
	// 	salt = salt->next;
	// }
		// printf ("seg search\n");
		// return ;
	if (salt->next != NULL)
	{
		// printf("In the statement\n");
		// return ;
		// if(salt->next->indentifier == INPUT_F)
		fd[0] = open(salt->next->arguments[0], O_RDONLY);
		// if(salt->next->indentifier == OUTPUT_F)
		fd[1] = open(salt->next->arguments[0], O_WRONLY | O_CREAT | O_TRUNC, 0664);
	}
	pid = fork();
	if (pid < 0)
	{
		freeing_chunks(&salt, info);
		// printf("terst: %d\n", info->d_quotes);
		printf("Error: while forking single child process.\n");
		return ;
	}
	if (pid == 0)
	{
		// run(salt, info, envp);
		roles(fd, salt, info, envp);
	}
	if (salt->next != NULL)
	{
	close(fd[0]);
	close(fd[1]);
	}
	waitpid(pid, NULL, 0);
}


