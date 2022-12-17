/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 14:09:07 by lkavalia          #+#    #+#             */
/*   Updated: 2022/12/17 00:03:37 by lkavalia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	builtins_env(char **arguments, t_env *e_list)
{
	if (arguments[1])
	{
		write(2, "\033[0;31menv: take your arguments/flags back!\033[0m\n", 49);
		return (1);
	}
	while (e_list != NULL && e_list != NULL)
	{
		printf("%s\n", e_list->var);
		e_list = e_list->next;
	}
	return (0);
}
