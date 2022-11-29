/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbiederm <pbiederm@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 09:05:13 by pbiederm          #+#    #+#             */
/*   Updated: 2022/11/14 09:20:25 by pbiederm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/get_next_line.h"

int	find_delim(t_chunk ** salt)
{
	t_chunk	*local_chunk;
	local_chunk = *salt;
	while(local_chunk != NULL)
	{	
		if (local_chunk->indentifier == DELIMITOR)
			return (1);
		local_chunk = local_chunk->next;
	}
	return (0);
}

void	here_doc(t_chunk	**salt, t_info *info, char	**envp)
{
	t_chunk	*local_chunk;
	char	*delimit;
	int		pfd[2];
	char*	buff;

	buff = malloc(9999);
	pipe(pfd);
	local_chunk = *salt;
	while(local_chunk != NULL)
	{	
		if (local_chunk->indentifier == DELIMITOR)
			break ;
		local_chunk = local_chunk->next;
	}
	delimit = local_chunk->arguments[0];
	printf("delimitor %s\n", delimit);
	while (TRUE)
	{
		write(2, "> ", 2);
		buff = get_next_line(STDIN_FILENO);
		if (ft_strncmp(buff, delimit, ft_strlen(delimit)) == 0)
			break;
		write(pfd[OUTPUT], buff, strlen(buff));
	}
	close(pfd[OUTPUT]);
	dup2(pfd[INPUT], STDIN_FILENO);
	close(pfd[INPUT]);
	free(buff);
	run(*salt, info, envp);
}

void	here_doc_multi(t_chunk	**salt)
{
	t_chunk	*local_chunk;
	char	*delimit;
	int		pfd[2];
	char*	buff;

	buff = malloc(9999);
	pipe(pfd);
	local_chunk = *salt;
	while(local_chunk != NULL)
	{	
		if (local_chunk->indentifier == DELIMITOR)
			break ;
		local_chunk = local_chunk->next;
	}
	delimit = local_chunk->arguments[0];
	while (TRUE)
	{
		buff = get_next_line(STDIN_FILENO);
		if (ft_strncmp(buff, delimit, ft_strlen(delimit)) == 0)
			break;
		write(pfd[OUTPUT], buff, strlen(buff));
	}
	close(pfd[OUTPUT]);
	dup2(pfd[INPUT], STDIN_FILENO);
	close(pfd[INPUT]);
	free(buff);
}
