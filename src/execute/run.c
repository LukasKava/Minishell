/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbiederm <pbiederm@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 11:49:51 by pbiederm          #+#    #+#             */
/*   Updated: 2022/11/07 11:04:11 by pbiederm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	run(t_chunk	*salt, t_info *info, char	**envp)
{
	// if (access(salt->command_path, F_OK) != 0)
	// {
	// 	// freeing_chunks(salt, info);
	// 	// printf("terst: %d\n", info->d_quotes);
	// 	printf("Error: command not found\n");
	// 	return ;
	// }
	if (execve(salt->command_path, salt->arguments, envp) < 0)
	{
		//needs free
		//need an idicator of error in the struct
		freeing_chunks(&salt, info);
		printf("Error when attempting to execute programme\n");
		return ;
	}
}