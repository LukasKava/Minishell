/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/17 13:22:51 by lkavalia          #+#    #+#             */
/*   Updated: 2022/12/18 16:46:35 by lkavalia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	check_len(t_env *exl)
{
	int	a;
	int	b;

	a = ft_strlen((*exl).var_name);
	b = ft_strlen((*exl).next->var_name);
	if (a > b)
		return (a);
	return (b);
}

int	name_exists(t_env *exp_list, char *str)
{
	char	*temp_name;

	temp_name = save_name(str);
	while (exp_list != NULL)
	{
		if (ft_strncmp(exp_list->var_name, temp_name, \
						ft_strlen(temp_name)) == 0)
		{
			free(temp_name);
			return (1);
		}
		exp_list = exp_list->next;
	}
	free(temp_name);
	return (0);
}

int	possible_cases(char *arg)
{
	int	i;
	int	checker;

	i = 0;
	checker = 0;
	if (valid_name(arg) == 1)
		return (-1);
	while (arg[i] != '\0')
	{
		if (arg[i] == '=')
		{
			if (i == 0)
				checker = -1;
			else
				checker = 1;
			break ;
		}
		i++;
	}
	return (checker);
}

void	inject_to_exp_l(t_env **exp_l, char *str)
{
	t_env	*temp;

	temp = (*exp_l);
	while ((*exp_l)->next != NULL)
		(*exp_l) = (*exp_l)->next;
	(*exp_l) = attach_end(*exp_l);
	(*exp_l) = (*exp_l)->next;
	(*exp_l)->var_name = save_name(str);
	(*exp_l)->var = ft_strdup(str);
	(*exp_l) = temp;
}

void	inject_to_e_l(t_env **env_l, char *str)
{
	t_env	*temp;

	temp = (*env_l);
	while ((*env_l)->next != NULL)
		(*env_l) = (*env_l)->next;
	(*env_l) = attach_end(*env_l);
	(*env_l) = (*env_l)->next;
	(*env_l)->var = ft_strdup(str);
	(*env_l)->var_name = save_name(str);
	(*env_l) = temp;
}
