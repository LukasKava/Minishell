/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/27 18:27:10 by lkavalia          #+#    #+#             */
/*   Updated: 2022/11/27 18:50:49 by lkavalia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void delete_exp_node(t_env **exp_l, char *str)
{
	t_env *temp_ex;
	t_env *temp_node;
	temp_ex = (*exp_l);
	temp_node = NULL;
	if (*exp_l != NULL && ft_strncmp((*exp_l)->var_name, str, ft_strlen(str)) == 0)
	{
		printf("fails here in0\n");
		(*exp_l) = (*exp_l)->next;
		printf("fails here in1\n");
		free(temp_ex->var_name);
		// printf("fails here in2\n");
		free(temp_ex->var);
		// printf("fails here in3\n");
		free(temp_ex);
		return;
	}
	while ((*exp_l) != NULL)
	{
		if ((*exp_l)->next != NULL && ft_strncmp((*exp_l)->next->var_name, str, ft_strlen(str)) == 0)
		{
			printf("fails inside\n");
			temp_node = (*exp_l)->next;
			(*exp_l)->next = (*exp_l)->next->next;
			free(temp_node->var_name);
			free(temp_node->var);
			free(temp_node);
			printf("fails after\n");
			break;
		}
		(*exp_l) = (*exp_l)->next;
	}
	(*exp_l) = temp_ex;
}

static void delete_env_node(t_env **e_l, char *str)
{
	t_env *temp_e;
	t_env *temp_node;
	char *name;
	temp_e = (*e_l);
	temp_node = NULL;
	name = save_name((*e_l)->var);
	if (*e_l != NULL && ft_strncmp(name, str, ft_strlen(str)) == 0)
	{
		(*e_l) = (*e_l)->next;
		free(temp_e->var_name);
		free(temp_e->var);
		free(name);
		free(temp_e);
		return;
	}
	while ((*e_l) != NULL)
	{
		if ((*e_l)->next != NULL)
			name = save_name((*e_l)->next->var);
		if ((*e_l)->next != NULL && ft_strncmp(name, str, ft_strlen(str)) == 0)
		{
			temp_node = (*e_l)->next;
			(*e_l)->next = (*e_l)->next->next;
			free(temp_node->var_name);
			free(temp_node->var);
			free(temp_node);
			free(name);
			break;
		}
		if (name != NULL)
			free(name);
		name = NULL;
		(*e_l) = (*e_l)->next;
	}
	(*e_l) = temp_e;
}

int builtins_unset(t_env **exp_l, t_env **env_l, char **line)
{
	int i;
	i = 0;
	while (line[i] != NULL)
	{
		if (i == 0)
			i++;
		printf("valid name: %d\n", valid_name(line[i]));
		if (line[i] != NULL && valid_name(line[i]) == 0)
		{
			printf("name: %s\n", line[i]);
			delete_exp_node(exp_l, line[i]);
			printf("fails after exp_node");
			delete_env_node(env_l, line[i]);
			printf("env.... %s\n", (*env_l)->var);
		}
		else
			return (1);
		i++;
	}
	return (0);
}