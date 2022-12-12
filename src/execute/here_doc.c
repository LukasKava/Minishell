/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbiederm <pbiederm@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 09:05:13 by pbiederm          #+#    #+#             */
/*   Updated: 2022/12/12 19:32:35 by pbiederm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	here_doc(char	*delimit)
{
	int		pfd[2];
	char*	buff;
	
	if (pipe(pfd) == -1)
	{
		g_exit_status = 1;
		write(2, "Problems with setting up the here doc pipe\n", 44);
	}
	buff = calloc(9999, sizeof(char));
	while (TRUE)
	{
		buff = readline("> ");
		if (!buff)
		{
			printf("CTRL D was activated!\n");
			return (1);
		}
		if (ft_strncmp(buff, delimit, ft_strlen(delimit)) == 0 &&
		(ft_strlen(delimit) == ft_strlen(buff)))
			break;
		write(pfd[OUTPUT], buff, strlen(buff));
		write(pfd[OUTPUT], "\n", 1);
	}
	close(pfd[OUTPUT]);
	free(buff);
	free(delimit);
	return(pfd[INPUT]);
}
