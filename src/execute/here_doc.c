/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbiederm <pbiederm@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 09:05:13 by pbiederm          #+#    #+#             */
/*   Updated: 2022/12/03 20:41:30 by pbiederm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/get_next_line.h"

int	here_doc(char	*delimit)
{
	int		pfd[2];
	char*	buff;

	pipe(pfd);
	while (TRUE)
	{
		buff = readline("> ");
		fprintf(stderr, "output: %s\n", buff);
		fprintf(stderr, "delimitor: %s strlen: %ld strncmp: %d\n", delimit, ft_strlen(delimit), ft_strncmp(buff, delimit, ft_strlen(delimit)));
		if (ft_strncmp(buff, delimit, ft_strlen(delimit)) == 0)
			break;
		write(pfd[OUTPUT], buff, strlen(buff));
		write(pfd[OUTPUT], "\n", 1);
	}
	close(pfd[OUTPUT]);
	free(buff);
	return(pfd[INPUT]);
}
