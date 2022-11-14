/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_output.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbiederm <pbiederm@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 09:14:20 by pbiederm          #+#    #+#             */
/*   Updated: 2022/11/14 09:25:28 by pbiederm         ###   ########.fr       */
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
	here_doc_multi(&salt);
	free_fd(fd);
	run(salt, info, envp);
}