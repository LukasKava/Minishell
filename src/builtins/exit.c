/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/27 18:35:18 by lkavalia          #+#    #+#             */
/*   Updated: 2022/11/27 18:37:48 by lkavalia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int builtins_exit(t_env **exp_l, t_env **env_l, char **line)
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
	while (line[1][i] != '\0')
	{
		if (line[1][i] >= '0' && line[1][i] <= '9')
			i++;
		else
		{
			write(2, "it is not a numeric argument!\nexit", 35);
			// set the global exit to 2
			break;
		}
	}
	// Free everything.
	printf("env exp: %s %s\n", (*exp_l)->var_name, (*env_l)->var);
	i = ft_atoi(line[1]) % 256;
	printf("i: %d\n", i);
	write(2, "exit\n", 6);
	exit(i); // I need to give it the global exit staatus
	return (0);
}