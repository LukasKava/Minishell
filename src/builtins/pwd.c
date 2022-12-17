/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 12:39:38 by lkavalia          #+#    #+#             */
/*   Updated: 2022/12/17 00:17:11 by lkavalia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	builtins_pwd(int fd)
{
	char	holder[PATH_MAX];

	if (!getcwd(holder, PATH_MAX))
	{
		perror("pwd: ");
		write(1, "\n", 1);
		g_errors.g_exit_status = 12;
		return (12);
	}
	ft_putstr_fd(holder, fd);
	write(1, "\n", 1);
	g_errors.g_exit_status = 0;
	return (0);
}
