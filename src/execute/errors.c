/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbiederm <pbiederm@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/17 11:29:40 by pbiederm          #+#    #+#             */
/*   Updated: 2022/12/19 08:16:53 by pbiederm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	pipe_error_execute(void)
{
	g_errors.g_exit_status = 1;
	perror(" ");
}

void	redirection_out_error(t_vars *vars)
{
	perror("Error: ");
	g_errors.g_exit_status = 1;
	vars->capture_redirection_error = 1;
	vars->capture_exit_flag = -1;
	vars->builtin_exit_fail = 1;
}
