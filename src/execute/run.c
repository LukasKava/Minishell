/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbiederm <pbiederm@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 11:49:51 by pbiederm          #+#    #+#             */
/*   Updated: 2022/12/14 16:59:30 by pbiederm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	run(t_chunk	*salt, char	**envp)
{
	if (execve(salt->command_path, salt->arguments, envp) < 0)
	{
		// freeing_tokens(hive->token);
		// freeing_chunks(&hive->c_arr);
		write(2, salt->arguments[0], ft_strlen(salt->arguments[0]));
		perror(" ");
		g_exit_status = 127;
		exit(127);
	}
}