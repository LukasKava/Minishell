/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbiederm <pbiederm@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 09:05:13 by pbiederm          #+#    #+#             */
/*   Updated: 2022/12/16 18:03:17 by pbiederm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	pipe_error(void)
{
	g_exit_status = 1;
	write(2, "Problems with setting up the here doc pipe\n", 44);
	perror(" ");
}

int	here_doc(char	*delimit)
{
	int		pfd[2];
	char	*buff;

	if (pipe(pfd) == -1)
		pipe_error();
	signal(SIGINT, handle_sigint);
	while (TRUE)
	{
		buff = readline("> ");
		if (!buff)
		{
			write(2, "\033[0;31mCtrl-D was activated\033[0m\n", 33);
			perror(" ");
			g_exit_status = 5;
			return (1);
		}
		if (ft_strncmp(buff, delimit, ft_strlen(delimit)) == 0 && \
		(ft_strlen(delimit) == ft_strlen(buff)))
			break ;
		write(pfd[OUTPUT], buff, strlen(buff));
		write(pfd[OUTPUT], "\n", 1);
	}
	close(pfd[OUTPUT]);
	return (pfd[INPUT]);
}
