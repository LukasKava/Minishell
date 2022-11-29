/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 14:09:07 by lkavalia          #+#    #+#             */
/*   Updated: 2022/11/23 14:29:38 by lkavalia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int builtins_env(char **arguments, t_env *e_list)
{
	if (arguments[1])
	{
		write(2, "\033[0;31menv does not accept any arguments or flags!\033[0m\n", 56);
		return (1);
	}
	while (e_list->next != NULL && e_list != NULL)
	{
		printf("%s\n", e_list->var);
		e_list = e_list->next;
	}
	return (0);
}
