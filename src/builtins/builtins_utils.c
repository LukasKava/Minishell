/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 14:06:12 by lkavalia          #+#    #+#             */
/*   Updated: 2022/12/04 12:57:10 by lkavalia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int valid_name(char *name)
{
	int i;
	int error;
	i = 0;
	error = 0;
	while (name[i] != '\0')
	{
		if (name[i] == '=')
			break;
		if (((name[i] >= 'a' && name[i] <= 'z') || (name[i] >= 'A' && name[i] <= 'Z')) || (name[i] >= '0' && name[i] <= '9') || name[i] == '_')
			i++;
		else
		{
			error = 1;
			break;
		}
	}
	return (error);
}

t_env	*attach_end(t_env *token)
{
	t_env	*temp;

	temp = ft_calloc(1, sizeof(t_env));
	if (!temp)
	{
		printf("allocation failed!\n");
		return (NULL);
	}
	temp->var = NULL;
	temp->next = NULL;
	token->next = temp;
	return (token);
}

void	create_e_list(t_env **e_list, char **env)
{
	int		i;
	t_env	*head;

	i = 0;
	(*e_list) = ft_calloc(1, sizeof(t_env));
	head = (*e_list);
	if (!head)
	{
		printf("allocation has failed!\n");
		g_exit_status = 3;
		return;
	}
	while (env[i + 1] != NULL)
	{
		(*e_list)->var = ft_strdup(env[i]);
		(*e_list)->var_name = save_name((*e_list)->var);
		(*e_list) = attach_end(*e_list);
		(*e_list) = (*e_list)->next;
		i++;
	}
	(*e_list)->var = ft_strdup(env[i]);
	(*e_list)->var_name = save_name((*e_list)->var);
	(*e_list) = head;
}

void freeing_e_list(t_env **e_list)
{
	t_env *free_token;

	free_token = (*e_list);
	while ((*e_list) != NULL)
	{
		free_token = (*e_list);
		//printf("env->var: %s\n", (*e_list)->var);
		free((*e_list)->var);
		free((*e_list)->var_name);
		(*e_list) = (*e_list)->next;
		free(free_token);
		//printf("env freed succesfully!\n");
	}
	free(*e_list);
}

char *save_name(char *str)
{
	int i;
	char *name;

	i = 0;
	name = NULL;
	while (str[i] != '\0' && str[i] != '=')
		i++;
	name = ft_calloc(i + 1, sizeof(char));
	name[i] = '\0';
	i--;
	while (i >= 0)
	{
		name[i] = str[i];
		i--;
	}
	return (name);
}
