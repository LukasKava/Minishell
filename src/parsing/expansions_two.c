/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions_two.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 03:39:04 by lkavalia          #+#    #+#             */
/*   Updated: 2022/11/17 03:54:03 by lkavalia         ###   ########.fr       */
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
		if (str[i] == '$')
			count++;
		i++;
	}
	return (count);
}

int	find_expansion(char *str)
{
	int		i;
	bool	expansion_found;

	i = 0;
	expansion_found = false;
	while (str[i] != '\0')
	{
		if (str[i] == '$' && str[i + 1] != '\0')
		{
			i++;
			expansion_found = true;
			while (str[i] != '\0' && str[i] != ' ' && str[i] != '$')
			{
				if (ft_case(str[i]) == 1 \
						|| str[i] == '_' || (str[i] >= '0' && str[i] <= '9'))
					i++;
				else
					return (2);
			}
			if (expansion_found == true)
				return (0);
		}
		i++;
	}
	return (1);
}

char	*save_var(char *token)
{
	int		i;
	int		len;
	char	*env_var;

	i = 0;
	len = 0;
	env_var = NULL;
	while (token[i] != '$')
		i++;
	i++;
	while (token[i] != '\0' && (token[i] != ' ' && token[i] != '$'))
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

size_t	env_var_excists(char *str, char **envp)
{
	int		i;
	char	*env_var;

	i = 0;
	env_var = save_var(str);
	while (envp[i] != NULL \
			&& envp[i] != ft_strnstr(envp[i], env_var, ft_strlen(env_var)))
		i++;
	if (envp[i] == NULL || envp[i][ft_strlen(env_var)] != '=')
	{
		printf("VAR does not excist!\n");
		free(env_var);
		return (1);
	}
	free(env_var);
	return (0);
}
