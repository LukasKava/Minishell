/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbiederm <pbiederm@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 09:05:13 by pbiederm          #+#    #+#             */
/*   Updated: 2022/12/12 17:35:37 by pbiederm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	here_doc(char	*delimit)
{
	int		pfd[2];
	char*	buff;
	char*	trimmed_delimit;
	char	*set;
	
	set = malloc(2);
	set[0] = '"' + 'a';
	set[1] = '\0';
	trimmed_delimit = ft_strtrim(delimit, set);
	pipe(pfd);
	buff = calloc(9999, sizeof(char));
	while (TRUE)
	{
		buff = readline("> ");
		if (!buff)
		{
			printf("CTRL D was activated!\n");
			return (1);
		}
		if (ft_strncmp(buff, trimmed_delimit, ft_strlen(trimmed_delimit)) == 0)
			break;
		write(pfd[OUTPUT], buff, strlen(buff));
		write(pfd[OUTPUT], "\n", 1);
	}
	close(pfd[OUTPUT]);
	free(buff);
	free(set);
	free(trimmed_delimit);
	return(pfd[INPUT]);
}
