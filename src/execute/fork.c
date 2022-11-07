/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbiederm <pbiederm@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 11:43:52 by pbiederm          #+#    #+#             */
/*   Updated: 2022/11/07 10:28:53 by pbiederm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// void	exec_handle(t_chunk	*salt, t_info *info, char	**envp)
// {
	// switch between the chunks
// }

void	to_program(t_chunk	*salt, t_info *info, char	**envp)
{
	int	fd;
	int pfd[2];

	fd = open("test.txt", O_RDONLY);
	if (pipe = pfd[2] < 0)
		{
			printf("Error: while creating the pipe.\n");
			return ;
		}

}

void	single_child(t_chunk	*salt, t_info *info, char	**envp)
{
	int		pid;

	pid = fork();
	if (pid < 0)
	{
		freeing_chunks(salt, info);
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
