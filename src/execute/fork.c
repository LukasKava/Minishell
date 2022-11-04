/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbiederm <pbiederm@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 11:43:52 by pbiederm          #+#    #+#             */
/*   Updated: 2022/11/04 13:00:54 by pbiederm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	single_child(t_chunk	*salt, t_info *info, char	**envp)
{
	int		pid;

	pid = fork();
	if (pid < 0)
	{
		freeing_chunks(salt, info);
		// printf("terst: %d\n", info->d_quotes);
		printf("Error: while forking single child process\n");
		return ;
	}
	if (pid == 0)
	{
		run(salt, info, envp);
	}
	waitpid(pid, NULL, 0);
}
