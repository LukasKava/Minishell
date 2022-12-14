/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/27 17:28:52 by lkavalia          #+#    #+#             */
/*   Updated: 2022/12/14 11:14:25 by lkavalia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void write_name(char *str, int fd)
{
	int i;

	i = 0;
	while (str[i] != '\0' && str[i] != '=')
	{
		ft_putchar_fd(str[i], fd);
		i++;
	}
	if (str[i] != '\0')
	{
		ft_putstr_fd("=\"", fd);
		if (str[i] == '=')
		{
			i++;
			while (str[i] != '\0')
			{
				ft_putchar_fd(str[i], fd);
				i++;
			}
		}
		ft_putchar_fd(34, fd);
	}
	ft_putchar_fd('\n', fd);
}

void print_export_l(t_env *ex_l, int fd)
{
	while (ex_l->next != NULL)
	{
		ft_putstr_fd("declare -x ", fd);
		write_name(ex_l->var, fd);
		ex_l = ex_l->next;
	}
}

int check_len(t_env *exl)
{
	int a = ft_strlen((*exl).var_name);
	int b = ft_strlen((*exl).next->var_name);
	if (a > b)
		return (a);
	return (b);
}

void swap_data(t_env **exl)
{
	char *t_v;
	char *t_v_n;

	t_v = ft_strdup((*exl)->var);
	t_v_n = ft_strdup((*exl)->var_name);
	free((*exl)->var);
	free((*exl)->var_name);
	(*exl)->var = ft_strdup((*exl)->next->var);
	(*exl)->var_name = ft_strdup((*exl)->next->var_name);
	free((*exl)->next->var);
	free((*exl)->next->var_name);
	(*exl)->next->var = ft_strdup(t_v);
	(*exl)->next->var_name = ft_strdup(t_v_n);
	free(t_v);
	free(t_v_n);
}

void sort_indexes(t_env **exl)
{
	t_env *temp;

	temp = (*exl);
	while ((*exl)->next != NULL)
	{
		if (ft_strncmp((*exl)->var_name, (*exl)->next->var_name, check_len(*exl)) > 0)
		{
			swap_data((exl));
			(*exl) = temp;
		}
		else
			(*exl) = (*exl)->next;
	}
	(*exl) = temp;
}

int possible_cases(char *arg)
{
	int i;
	int checker;
	i = 0;
	checker = 0;
	if (valid_name(arg) == 1)
		return (-1);
	while (arg[i] != '\0')
	{
		if (arg[i] == '=')
		{
			checker = 1;
			break;
		}
		i++;
	}
//	printf("checker: %d\n", checker);
	return (checker);
}

void inject_to_exp_l(t_env **exp_l, char *str)
{
	t_env *temp;
	temp = (*exp_l);
	while ((*exp_l)->next != NULL)
		(*exp_l) = (*exp_l)->next;
	(*exp_l) = attach_end(*exp_l);
	(*exp_l) = (*exp_l)->next;
	(*exp_l)->var_name = save_name(str);
	(*exp_l)->var = ft_strdup(str);
	(*exp_l) = temp;
}

void inject_to_e_l(t_env **env_l, char *str)
{
	t_env *temp;
	temp = (*env_l);
	while ((*env_l)->next != NULL)
		(*env_l) = (*env_l)->next;
	(*env_l) = attach_end(*env_l);
	(*env_l) = (*env_l)->next;
	(*env_l)->var = ft_strdup(str);
	(*env_l)->var_name = save_name(str);
	(*env_l) = temp;
}

int name_exists(t_env *exp_list, char *str)
{
	char *temp_name;
	temp_name = save_name(str);
	while (exp_list != NULL)
	{
		if (ft_strncmp(exp_list->var_name, temp_name, ft_strlen(temp_name)) == 0)
		{
	//		printf("HELLO THE VARS MATCH!\n");
			free(temp_name);
			return (1);
		}
		exp_list = exp_list->next;
	}
	free(temp_name);
	return (0);
}

void replace_the_value(t_env **ex_l, t_env **e_l, char *str)
{
	char *name;
	char *e_name;
	t_env *temp_ex;
	t_env *temp_e;
	name = save_name(str);
	e_name = NULL;
	temp_ex = (*ex_l);
	temp_e = (*e_l);
	while ((*ex_l) != NULL)
	{
		if (ft_strncmp((*ex_l)->var_name, name, ft_strlen(name)) == 0)
		{
			free((*ex_l)->var_name);
			(*ex_l)->var_name = ft_strdup(name);
			free((*ex_l)->var);
			(*ex_l)->var = ft_strdup(str);
			break;
		}
		(*ex_l) = (*ex_l)->next;
	}
	(*ex_l) = temp_ex;
	while ((*e_l) != NULL)
	{
		e_name = save_name((*e_l)->var);
		if (ft_strncmp(e_name, name, ft_strlen(name)) == 0)
		{
			free((*e_l)->var);
			(*e_l)->var = ft_strdup(str);
			free(e_name);
			break;
		}
		free(e_name);
		(*e_l) = (*e_l)->next;
	}
	(*e_l) = temp_e;
	free(name);
}

int builtins_export(t_env **exp_list, t_env **e_l, char **line, int fd)
{
	int i;
	int a;

	i = -1;
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
			i = possible_cases(line[a]);
	//		printf("case: %s valid: %d\n", line[a], i);
			if (i == -1)
				return (1);
			if (name_exists(*exp_list, line[a]) == 1)
				replace_the_value(exp_list, e_l, line[a]);
			else if (i == 1)
			{
				inject_to_exp_l(exp_list, line[a]);
				inject_to_e_l(e_l, line[a]);
			}
			if (i == 0 && name_exists(*exp_list, line[a]) != 1)
				inject_to_exp_l(exp_list, line[a]);
			a++;
		}
	}
	return (0);
}
