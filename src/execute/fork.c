/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbiederm <pbiederm@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 11:43:52 by pbiederm          #+#    #+#             */
/*   Updated: 2022/11/07 11:47:31 by pbiederm         ###   ########.fr       */
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
