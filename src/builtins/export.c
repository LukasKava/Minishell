/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/27 17:28:52 by lkavalia          #+#    #+#             */
/*   Updated: 2022/12/18 17:13:54 by lkavalia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	ex_l_core(t_env **ex_l, char *name, char *str)
{
	if (ft_strncmp((*ex_l)->var_name, name, ft_strlen(name)) == 0)
	{
		free((*ex_l)->var_name);
		(*ex_l)->var_name = ft_strdup(name);
		free((*ex_l)->var);
		(*ex_l)->var = ft_strdup(str);
		free((*ex_l)->trim_var);
		(*ex_l)->trim_var = ft_strtrim_f(str, name);
		return (1);
	}
	return (0);
}

static int	e_l_core(t_env **e_l, char *name, char *str)
{
	char	*e_name;

	e_name = save_name((*e_l)->var);
	if (ft_strncmp(e_name, name, ft_strlen(name)) == 0)
	{
		free((*e_l)->var);
		(*e_l)->var = ft_strdup(str);
		free(e_name);
		return (1);
	}
	free(e_name);
	return (0);
}

void	replace_the_value(t_env **ex_l, t_env **e_l, char *str)
{
	char	*name;
	t_env	*temp_ex;
	t_env	*temp_e;

	name = save_name(str);
	temp_ex = (*ex_l);
	temp_e = (*e_l);
	while ((*ex_l) != NULL)
	{
		if (ex_l_core(ex_l, name, str) == 1)
			break ;
		(*ex_l) = (*ex_l)->next;
	}
	(*ex_l) = temp_ex;
	while ((*e_l) != NULL)
	{
		if (e_l_core(e_l, name, str))
			break ;
		(*e_l) = (*e_l)->next;
	}
	(*e_l) = temp_e;
	free(name);
}

static int	brains_export(t_env **exp_l, t_env **e_l, int a, char **line)
{
	int	i;

	i = possible_cases(line[a]);
	if (i == -1)
	{
		write(2, "\033[0;31mNot a valid indentifier!\033[0m\n", 37);
		return (1);
	}
	if (name_exists(*e_l, line[a]) == 1 && i == 0)
		delete_env_node(e_l, line[a]);
	if (name_exists(*exp_l, line[a]) == 1)
	{
		replace_the_value(exp_l, e_l, line[a]);
		if (i == 1 && name_exists(*e_l, line[a]) == 0)
			inject_to_e_l(e_l, line[a]);
	}
	else if (i == 1)
	{
		inject_to_exp_l(exp_l, line[a]);
		inject_to_e_l(e_l, line[a]);
	}
	if (i == 0 && name_exists(*exp_l, line[a]) != 1)
		inject_to_exp_l(exp_l, line[a]);
	return (0);
}

int	builtins_export(t_env **exp_list, t_env **e_l, char **line, int fd)
{
	int	a;

	a = 1;
	if (!line[1])
	{
		sort_indexes(exp_list);
		print_export_l(*exp_list, fd);
	}
	else
	{
		while (line[a] != NULL)
		{
			brains_export(exp_list, e_l, a, line);
			a++;
		}
	}
	return (0);
}
