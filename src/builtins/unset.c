/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/27 18:27:10 by lkavalia          #+#    #+#             */
/*   Updated: 2022/12/17 22:24:29 by lkavalia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	free_first_node(t_env **exp_l, t_env **temp_ex)
{
	(*exp_l) = (*exp_l)->next;
	free((*temp_ex)->trim_var);
	free((*temp_ex)->var_name);
	free((*temp_ex)->var);
	free(*temp_ex);
}

static void	free_node(t_env **exp_l, t_env **temp_node)
{
	(*temp_node) = (*exp_l)->next;
	(*exp_l)->next = (*exp_l)->next->next;
	free((*temp_node)->trim_var);
	free((*temp_node)->var_name);
	free((*temp_node)->var);
	free(*temp_node);
}

static void	delete_exp_node(t_env **exp_l, char *str)
{
	t_env	*temp_ex;
	t_env	*temp_node;

	temp_ex = (*exp_l);
	temp_node = NULL;
	if (*exp_l != NULL && \
		ft_strncmp((*exp_l)->var_name, str, ft_strlen(str)) == 0)
	{
		free_first_node(exp_l, &temp_ex);
		return ;
	}
	while ((*exp_l) != NULL)
	{
		if ((*exp_l)->next != NULL && \
			ft_strncmp((*exp_l)->next->var_name, str, ft_strlen(str)) == 0)
		{
			free_node(exp_l, &temp_node);
			break ;
		}
		(*exp_l) = (*exp_l)->next;
	}
	(*exp_l) = temp_ex;
}

void	delete_env_node(t_env **e_l, char *str)
{
	t_env	*temp_e;
	t_env	*temp_node;

	temp_e = (*e_l);
	temp_node = NULL;
	if (*e_l != NULL && ft_strncmp((*e_l)->var_name, str, ft_strlen(str)) == 0)
	{
		free_first_node(e_l, &temp_e);
		return ;
	}
	while ((*e_l) != NULL)
	{
		if ((*e_l)->next != NULL && \
			ft_strncmp((*e_l)->next->var_name, str, ft_strlen(str)) == 0)
		{
			free_node(e_l, &temp_node);
			break ;
		}
		(*e_l) = (*e_l)->next;
	}
	(*e_l) = temp_e;
}

int	builtins_unset(t_env **exp_l, t_env **env_l, char **line)
{
	int	i;

	i = 0;
	while (line[i] != NULL)
	{
		if (i == 0)
			i++;
		if (line[i] != NULL && valid_name(line[i]) == 0)
		{
			delete_exp_node(exp_l, line[i]);
			delete_env_node(env_l, line[i]);
		}
		else
			return (1);
		i++;
	}
	return (0);
}
