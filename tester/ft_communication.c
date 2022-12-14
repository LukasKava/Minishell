/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_communication.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkaufman <rkaufman@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/02 23:29:58 by rkaufman          #+#    #+#             */
/*   Updated: 2022/04/04 20:09:48 by rkaufman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_tester.h"

void	ft_send_to_minishell(t_data *data, char *cmd)
{
	write(data->main_mini.fd_write, cmd, ft_strlen(cmd));
}

void	ft_send_to_bash(t_data *data, char *cmd)
{
	write(data->main_bash.fd_write, cmd, ft_strlen(cmd));
}

char	*ft_read_from_minishell(t_data *data)
{
	return (ft_get_next_line(data->main_mini.fd_read));
}

char	*ft_read_from_bash(t_data *data)
{
	return (ft_get_next_line(data->main_bash.fd_read));
}

char	*ft_get_next_cmd(t_data *data)
{
	char	*output;

	output = ft_get_next_line(data->fd_tests);
	while (output)
	{
		if (output[0] != '#' && output[0] != '\n')
			return (output);
		if (output[0] == '#')
		{
			/*
			ft_write_fd(1, COLOR_MAGENTA);
			ft_write_fd(1, output);
			ft_write_fd(1, COLOR_DEFAULT);
			*/
		}
		free(output);
		output = ft_get_next_line(data->fd_tests);
	}
	return (output);
}