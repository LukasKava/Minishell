/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbiederm <pbiederm@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 19:09:40 by lkavalia          #+#    #+#             */
/*   Updated: 2022/12/19 12:18:28 by pbiederm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	echo_str_cmp(char **line)
{
	if (line[1] != NULL)
	{
		if (line[1][0] == '-')
		{
			if (line[1][1] != '\0' && line[1][1] == 'n')
			{
				if (line[1][2] == '\0')
					return (0);
			}
		}
	}
	return (1);
}

static void	while_loop(char **line, int i, int fd)
{
	while (line[i] != NULL)
	{
		ft_putstr_fd(line[i], fd);
		if (line[i + 1] != NULL)
			write(fd, " ", 1);
		i++;
	}
}

int	builtins_echo(int fd, char **line, t_vars *vars)
{
	int	i;

	i = 1;
	if (echo_str_cmp(line) == 0)
	{
		i++;
		while_loop(line, i, fd);
	}
	else
	{
		while_loop(line, i, fd);
		write(fd, "\n", 1);
	}
	if (!vars->capture_redirection_error)
		g_errors.g_exit_status = 0;
	vars->capture_redirection_error = 0;
	return (0);
}
