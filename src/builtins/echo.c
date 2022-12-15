/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 19:09:40 by lkavalia          #+#    #+#             */
/*   Updated: 2022/12/14 23:20:54 by lkavalia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int echo_str_cmp(char **line)
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

int builtins_echo(int fd, char **line)
{
	int i;

	i = 1;
	if (echo_str_cmp(line) == 0)
	{
		i++;
		while (line[i] != NULL)
		{
			ft_putstr_fd(line[i], fd);
			if (line[i + 1] != NULL)
				write(fd, " ", 1);
			i++;
		}
	}
	else
	{
		while (line[i] != NULL)
		{
			ft_putstr_fd(line[i], fd);
			if (line[i + 1] != NULL)
				write(fd, " ", 1);
			i++;
		}
		write(fd, "\n", 1);
	}
	return (0);
}