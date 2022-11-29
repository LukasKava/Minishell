/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 12:39:38 by lkavalia          #+#    #+#             */
/*   Updated: 2022/11/27 18:49:00 by lkavalia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int builtins_pwd(int fd)
{
	char holder[PATH_MAX];

	if (!getcwd(holder, PATH_MAX))
	{
		perror("pwd: ");
		write(1, "\n", 1);
		return (1);
	}
	ft_putstr_fd(holder, fd);
	write(1, "\n", 1);
	return (0);
}