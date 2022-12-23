/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/18 14:24:58 by lkavalia          #+#    #+#             */
/*   Updated: 2022/12/23 19:35:55 by lkavalia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	handle_sigint(int sig)
{
	if (sig == SIGINT)
	{
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		g_errors.g_exit_status = 130;
	}
}

void	handle_child(int sig)
{
	if (sig == SIGINT && g_errors.bip == false)
	{
		g_errors.bip = true;
		write(1, "\n", 1);
		g_errors.g_exit_status = 130;
	}
}
