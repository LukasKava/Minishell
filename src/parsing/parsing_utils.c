/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/17 23:45:04 by lkavalia          #+#    #+#             */
/*   Updated: 2022/12/18 00:26:50 by lkavalia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*path_checker(t_env *env)
{
	char *path;

	path = NULL;	
	if (env == NULL)
	{
		printf("\033[0;31mERROR in parsing.c PATH is not excistent!\033[0m\n");
		return (NULL);
	}
	else
		path = env->var;
	return (path);
}

int	find_correct_path(char **splitted_path, char *s)
{
	int	i;

	i = 0;
	while (splitted_path[i] != NULL)
	{
		splitted_path[i] = ft_strjoin(splitted_path[i], "/");
		splitted_path[i] = ft_strjoin(splitted_path[i], s);
		if (access(splitted_path[i], F_OK) == 0 &&
			access(splitted_path[i], X_OK) == 0)
			break;
		free(splitted_path[i]);
		i++;
	}
	return (i);
}
