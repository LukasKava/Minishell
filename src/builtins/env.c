/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbiederm <pbiederm@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 14:09:07 by lkavalia          #+#    #+#             */
/*   Updated: 2022/12/18 17:30:50 by pbiederm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	builtins_env(char **arguments, t_env *e_list, t_vars *vars)
{
	if (arguments[1])
	{
		write(2, "\033[0;31menv: take your arguments/flags back!\033[0m\n", 49);
		g_errors.g_exit_status = 1;
		vars->capture_exit_flag = 0;
		return (1);
	}
	while (e_list != NULL && e_list != NULL)
	{
		printf("%s\n", e_list->var);
		e_list = e_list->next;
	}
	return (0);
}
