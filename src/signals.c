/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/18 14:24:58 by lkavalia          #+#    #+#             */
/*   Updated: 2022/12/14 11:28:38 by lkavalia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	handle_sigint(int sig)
{
	if (sig == SIGINT)
	{
		write(1, "hey\n", 5);
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

void	handle_child(int sig)
{
	if (sig == SIGINT && bip == false)
	{
		//printf("insidesdhf");
	//	rl_replace_line("", 0);
	//	printf("\n");
		bip = true;
		write(1, "djkkfks\n", 9);
		//exit(1);
		//exit(1);
	}
}

// void handle_child(int sig)
// {
// 	if (sig == SIGINT)
// 	{
// 		// printf("insidesdhf");
// 		//	rl_replace_line("", 0);
// 		write(1, "\n", 1);
// 		write(1, "here\n", 9);
// 		// exit(1);
// 		// exit(1);
// 	}
// }