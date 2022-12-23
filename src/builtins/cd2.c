/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd2.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/16 20:30:30 by lkavalia          #+#    #+#             */
/*   Updated: 2022/12/23 03:40:22 by lkavalia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	check_s_c(char **line)
{
	if (line[1][0] == '-' && line[1][1] == '\0')
		return (1);
	else if (line[1][0] == '~' && line[1][1] == '\0')
		return (2);
	else if (line[1][0] == '~' && line[1][1] != '\0')
		return (3);
	return (0);
}

char	*get_home(t_env **e)
{
	t_env	*temp;
	char	*home;

	temp = (*e);
	home = NULL;
	while ((*e) != NULL && (*e)->var != ft_strnstr((*e)->var, "HOME", 7))
		(*e) = (*e)->next;
	if ((*e) == NULL)
	{
		write(2, "\033[0;31mCannot find the HOME\n\033[0m", 33);
		g_errors.g_exit_status = 10;
		return (NULL);
	}
	home = (*e)->trim_var;
	(*e) = temp;
	return (home);
}

char	*all_cases(t_env **e, char **line, int spec)
{
	char	*combination;
	char	*temp;
	char	*second_temp;

	combination = NULL;
	temp = NULL;
	second_temp = NULL;
	if (spec == 1)
		return (get_env_prev(e));
	if (spec == 2)
		return (get_home(e));
	if (spec == 3)
	{
		if (line[1][0] == '~' && line[1][1] == '/')
			second_temp = ft_strtrim_f(line[1], "~/");
		else
			second_temp = ft_strtrim_f(line[1], "~");
		temp = ft_strdup("/home/");
		combination = ft_strjoin(temp, second_temp);
		free(second_temp);
		return (combination);
	}
	return (NULL);
}

char	*get_env_prev(t_env **e)
{
	t_env	*temp;
	char	*old_pwd;

	temp = (*e);
	old_pwd = NULL;
	while ((*e) != NULL && (*e)->var != ft_strnstr((*e)->var, "OLDPWD", 7))
		(*e) = (*e)->next;
	if ((*e) == NULL)
	{
		write(2, "\033[0;31mCannot find the OLDPWD\n\033[0m", 35);
		g_errors.g_exit_status = 10;
		return (NULL);
	}
	old_pwd = ft_strtrim_f((*e)->var, "OLDPWD=");
	(*e) = temp;
	return (old_pwd);
}
