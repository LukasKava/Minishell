/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/27 18:35:18 by lkavalia          #+#    #+#             */
/*   Updated: 2022/12/16 12:42:16 by lkavalia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int checking_digit(char *nbr)
{
	int i;

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

int builtins_exit(t_data *hive, char **line)
{
	int i;
	i = 0;
	if (line[1] == NULL)
		exit(0);
	if (line[2] != NULL)
	{
		write(2, "too many arguments!\n", 21);
		return (1);
	}
	if (checking_digit(line[1]) == -1)
	{
		write(2, "it is not a numeric argument!\nexit", 35);
		g_exit_status = 2;
		exit(g_exit_status);
	}
	i = ft_atoi(line[1]) % 256;
	g_exit_status = i;
	freeing_tokens(hive->token);
	freeing_chunks(&hive->c_arr);
	freeing_e_list(&hive->env);
	freeing_e_list(&hive->exp_l);
	write(2, "exit\n", 6);
	exit(i);
}