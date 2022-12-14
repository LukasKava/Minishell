/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/27 18:35:18 by lkavalia          #+#    #+#             */
/*   Updated: 2022/12/20 16:39:37 by lkavalia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	checking_digit(char *nbr)
{
	int	i;

	i = 0;
	if (nbr[i] == '-' || (nbr[i] >= '0' && nbr[i] <= '9') || nbr[i] == '+')
	{
		i++;
		while (nbr[i] != '\0' && (nbr[i] >= '0' && nbr[i] <= '9'))
			i++;
		if (nbr[i] != '\0')
			return (-1);
		else
			return (0);
	}
	return (-1);
}

int	builtins_exit(t_data *hive, char **line)
{
	write(1, "exit\n", 6);
	if (line[1] == NULL)
		exit(0);
	if (line[2] != NULL)
	{
		write(2, "\033[0;31mexit: take your arguments back!\n\033[0m", 44);
		g_errors.g_exit_status = 11;
		return (11);
	}
	if (checking_digit(line[1]) == -1)
	{
		write(2, "\033[0;31mI only like numbers!\n\033[0m", 33);
		g_errors.g_exit_status = 2;
		free(hive->info.r);
		exit(g_errors.g_exit_status);
	}
	g_errors.g_exit_status = ft_atoi(line[1]) % 256;
	freeing_tokens(hive->token);
	freeing_chunks(&hive->c_arr);
	freeing_e_list(&hive->exp_l);
	freeing_e_list(&hive->env);
	free(hive->info.r);
	exit(g_errors.g_exit_status);
}
