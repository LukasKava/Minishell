/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipes.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkaufman <rkaufman@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/02 23:31:17 by rkaufman          #+#    #+#             */
/*   Updated: 2022/04/03 10:49:49 by rkaufman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_tester.h"

int	ft_create_pipes(t_data *data)
{
	int	fd[2];

	if (pipe(fd) == -1)
		return (0);
	data->mini_main.fd_read = fd[0];
	data->main_mini.fd_write = fd[1];
	if (pipe(fd) == -1)
		return (0);
	data->main_mini.fd_read = fd[0];
	data->mini_main.fd_write = fd[1];
	if (pipe(fd) == -1)
		return (0);
	data->bash_main.fd_read = fd[0];
	data->main_bash.fd_write = fd[1];
	if (pipe(fd) == -1)
		return (0);
	data->main_bash.fd_read = fd[0];
	data->bash_main.fd_write = fd[1];
	return (1);
}

void	ft_close_main_pipes(t_data *data)
{
	close(data->main_mini.fd_read);
	close(data->main_mini.fd_write);
	close(data->main_bash.fd_read);
	close(data->main_bash.fd_write);
}

void	ft_close_pipes(t_data *data)
{
	close(data->main_mini.fd_read);
	close(data->main_mini.fd_write);
	close(data->main_bash.fd_read);
	close(data->main_bash.fd_write);
	close(data->fd_tests);
}
