/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 14:06:12 by lkavalia          #+#    #+#             */
/*   Updated: 2022/12/19 17:02:46 by lkavalia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	valid_name(char *name)
{
	int	i;
	int	error;

	i = 0;
	error = 0;
	if (name[i] == '\0' || (name[i] >= '0' && name[i] <= '9'))
		return (1);
	while (name[i] != '\0')
	{
		if (name[i] == '=')
			break ;
		if (((name[i] >= 'a' && name[i] <= 'z') || \
			(name[i] >= 'A' && name[i] <= 'Z')) || \
			(name[i] >= '0' && name[i] <= '9') || name[i] == '_')
			i++;
		else
		{
			error = 1;
			break ;
		}
	}
	return (error);
}

t_env	*attach_end(t_env *token)
{
	t_env	*temp;

	if (!token)
	{
		write(2, "Invalid token", 14);
		g_errors.g_exit_status = 3;
		return (NULL);
	}
	temp = ft_calloc(1, sizeof(t_env));
	if (!temp)
	{
		printf("\033[0;31mallocation failed!\033[0m\n");
		g_errors.g_exit_status = 3;
		return (NULL);
	}
	temp->var = NULL;
	temp->var_name = NULL;
	temp->trim_var = NULL;
	temp->next = NULL;
	token->next = temp;
	return (token);
}

static void	actions(t_env **e_list, char **env, char *temp, int i)
{
	(*e_list)->var = ft_strdup(env[i]);
	(*e_list)->var_name = save_name(env[i]);
	(*e_list)->trim_var = ft_strtrim_f((*e_list)->var, (*e_list)->var_name);
	temp = ft_strdup((*e_list)->trim_var);
	free((*e_list)->trim_var);
	(*e_list)->trim_var = ft_strtrim_f(temp, "=");
	free(temp);
	(*e_list) = attach_end(*e_list);
}

void	create_e_list(t_env **e_list, char **env)
{
	int		i;
	t_env	*head;
	char	*temp;

	i = 0;
	temp = NULL;
	(*e_list) = ft_calloc(1, sizeof(t_env));
	head = (*e_list);
	if (!head)
	{
		printf("\033[0;31mAllocation has failed!\033[0m\n");
		g_errors.g_exit_status = 3;
		return ;
	}
	while (env[i + 1] != NULL)
	{
		actions(e_list, env, temp, i);
		(*e_list) = (*e_list)->next;
		i++;
	} 
	(*e_list)->var = ft_strdup(env[i]);
	(*e_list)->var_name = save_name(env[i]);
	(*e_list) = head;
}

char	*save_name(char *str)
{
	int		i;
	char	*name;

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
