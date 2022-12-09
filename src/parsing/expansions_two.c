/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions_two.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 03:39:04 by lkavalia          #+#    #+#             */
/*   Updated: 2022/12/06 14:30:27 by lkavalia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

size_t	exp_count(char *str)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '$' && str[i + 1] != '\0' && ((str[i + 1] >= 'a' && str[i + 1] <= 'z') || \
			(str[i + 1] >= 'A' && str[i + 1] <= 'Z') || \
			(str[i + 1] >= '0' && str[i + 1] <= '9') || str[i + 1] == '_'))
			count++;
		i++;
	}
	return (count);
}

size_t en_excists(char *str, t_env *env)
{
	while (env != NULL && env->var != ft_strnstr(env->var, str, ft_strlen(str)))
		env = env->next;
	if (env == NULL || (env->var[ft_strlen(str)] != '=' && env->var[ft_strlen(str)] != '\0'))
	{
		printf("VAR does not excist!\n");
		g_exit_status = 127;
		return (1);
	}
	return (0);
}

char *save_ex_var(char *token)
{
	int i;
	int len;
	char *env_var;

	i = 0;
	len = 0;
	env_var = NULL;
	while (token[i] != '$')
		i++;
	i++;
	while (token[i] != '\0' && ((token[i] >= 'a' && token[i] <= 'z') ||
								(token[i] >= 'A' && token[i] <= 'Z') || (token[i] >= '0' && token[i] <= '9') || token[i] == '_'))
	{
		len++;
		i++;
	}
	env_var = ft_calloc(len + 1, sizeof(char));
	env_var[len] = '\0';
	len--;
	i--;
	while (len >= 0)
		env_var[len--] = token[i--];
	return (env_var);
}

char *return_ex_value(char *var, t_env *env)
{
	char *full_var;

	while (env != NULL && env->var != ft_strnstr(env->var, var, ft_strlen(var)))
		env = env->next;
	if (env == NULL || (env->var[ft_strlen(var)] != '=' && env->var[ft_strlen(var)] != '\0'))
	{
		printf("VAR does not excist!\n");
		g_exit_status = 127;
		free(var);
		return (NULL);
	}
	if (env->var[ft_strlen(var)] == '=')
		var = ft_strjoin(var, "=");
	full_var = env->var;
	full_var = ft_strtrim_beginning(full_var, var);
	free(var);
	return (full_var);
}
