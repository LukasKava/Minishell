/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbiederm <pbiederm@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 12:39:38 by lkavalia          #+#    #+#             */
/*   Updated: 2022/12/18 14:20:45 by pbiederm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	builtins_pwd(int fd, t_vars *vars)
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
	if (!vars->capture_redirection_error)
		g_errors.g_exit_status = 0;
	vars->capture_redirection_error = 0;
	return (0);
}
