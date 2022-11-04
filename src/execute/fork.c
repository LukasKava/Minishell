/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbiederm <pbiederm@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 11:43:52 by pbiederm          #+#    #+#             */
/*   Updated: 2022/11/04 12:20:58 by pbiederm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void single_child(t_chunk	*salt, char	**envp)
{
	int		pid;

	pid = fork();
	if (pid < 0)
	{
		printf("Error: while forking single child process\n");
	}
	if (pid == 0)
	{
		run(salt, envp);
	}
	waitpid(pid, NULL, 0);
	// collect_garbage(s)
}

// void collect_garbage