/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/06 14:56:46 by lkavalia          #+#    #+#             */
/*   Updated: 2022/12/16 17:50:23 by lkavalia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	simple_err_message(t_info *info, char *message, int exit_status)
{
	printf("\033[0;31m%s\033[0m\n", message);
	info->error = true;
	g_errors.g_exit_status = exit_status;
}
