/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/18 14:24:58 by lkavalia          #+#    #+#             */
/*   Updated: 2022/12/19 15:01:46 by lkavalia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	handle_sigint(int sig)
{
//	write(1, "parent activated!\n", 19);
	if (sig == SIGINT)
	{
		write(1, "parent\n", 8);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		g_errors.g_exit_status = 130;
	}
}

void	handle_child(int sig)
{
	write(1, "child activated!\n", 18);
	if (sig == SIGINT && g_errors.bip == false)
	{
		write(1, "child\n", 7);
		g_errors.bip = true;
		g_errors.g_exit_status = 130;
	}
}
