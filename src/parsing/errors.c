/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/06 14:56:46 by lkavalia          #+#    #+#             */
/*   Updated: 2022/12/06 15:20:05 by lkavalia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	simple_err_message(t_info *info, char *message, int exit_status)
{
	printf("%s\n", message);
	info->error = true;
	g_exit_status = exit_status;
}