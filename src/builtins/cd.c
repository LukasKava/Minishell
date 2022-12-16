/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 14:04:01 by lkavalia          #+#    #+#             */
/*   Updated: 2022/12/16 16:02:32 by lkavalia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	reset_old_pwd(t_env **e)
{
	t_env *temp;
	char *curr_pwd;

	temp = (*e);
	while ((*e) != NULL && (*e)->var != ft_strnstr((*e)->var, "PWD", 4))
		(*e) = (*e)->next;
	if ((*e) == NULL)
		return;
	curr_pwd = ft_strtrim_beginning((*e)->var, "PWD=");
	(*e) = temp;
	while ((*e) != NULL && (*e)->var != ft_strnstr((*e)->var, "OLDPWD", 7))
		(*e) = (*e)->next;
	if ((*e) == NULL)
		return;
	free((*e)->var);
	(*e)->var = ft_strdup("OLDPWD=");
	(*e)->var = ft_strjoin((*e)->var, curr_pwd);
	free(curr_pwd);
	(*e) = temp;
}

static void	reset_pwd(t_env **e)
{
	char curr_dir[PATH_MAX];
	char *pwd;
	t_env *temp;

	pwd = NULL;
	temp = (*e);
	while ((*e) != NULL && (*e)->var != ft_strnstr((*e)->var, "PWD", 7))
		(*e) = (*e)->next;
	if ((*e) == NULL)
		return;
	free((*e)->var);
	pwd = getcwd(curr_dir, PATH_MAX);
	(*e)->var = ft_strdup("PWD=");
	(*e)->var = ft_strjoin((*e)->var, pwd);
	//	free(pwd);
	(*e) = temp;
}

static char *get_env_prev(t_env **e)
{
	t_env *temp;
	char *old_pwd;

	temp = (*e);
	old_pwd = NULL;
	while ((*e) != NULL && (*e)->var != ft_strnstr((*e)->var, "OLDPWD", 7))
		(*e) = (*e)->next;
	if ((*e) == NULL)
	{
		write(2, "Cannot find the OLDPWD", 17);
		return (NULL);
	}
	old_pwd = ft_strtrim_beginning((*e)->var, "OLDPWD=");
	(*e) = temp;
	return (old_pwd);
}

char *get_home(t_env **e)
{
	t_env *temp;
	char *home;

	temp = (*e);
	home = NULL;
	while ((*e) != NULL && (*e)->var != ft_strnstr((*e)->var, "HOME", 7))
		(*e) = (*e)->next;
	if ((*e) == NULL)
	{
		write(2, "Cannot find the HOME", 21);
		return (NULL);
	}
	home = ft_strtrim_beginning((*e)->var, "HOME=");
	(*e) = temp;
	return (home);
}

char	*all_cases(t_env **e, char **line, int spec)
{
	char	*combination;
	char	*temp;

	combination = NULL;
	temp = NULL;
	if (spec == 1)
		return (get_env_prev(e));
	if (spec == 2)
		return (get_home(e));
	if (spec == 3)
	{
//		printf("spec 3\n");
		if (line[1][0] == '~' && line[1][1] == '/')
			combination = ft_strtrim_beginning(line[1], "~/");
		else 
			combination = ft_strtrim_beginning(line[1], "~");
		temp = ft_strdup("/home/");
		combination = ft_strjoin(temp, combination);
		//free(temp);
		return (combination);
	}
	return (NULL);
}

int check_s_c(char **line)
{
	if (line[1][0] == '-' && line[1][1] == '\0')
		return (1);
	else if (line[1][0] == '~' && line[1][1] == '\0')
		return (2);
	else if (line[1][0] == '~' && line[1][1] != '\0')
		return (3);
	return (0);
}

int builtins_cd(char **line, t_env **e_list)
{
	char *super_case;

	super_case = NULL;
	if (!line[1])
	{
		if (chdir(get_home(e_list)) != 0)
		{
			perror("chdir: ");
			return (1);
		}
		reset_old_pwd(e_list);
		reset_pwd(e_list);
	}
	else if (line[2] != NULL)
	{
		printf("TOO many arguments!\n");
		return (1);
	}
	else
	{
//		printf("fails before:\n");
		if (check_s_c(line) != 0)
		{
			super_case = all_cases(e_list, line, check_s_c(line));
//			printf("super_case: %s\n", super_case);
			if (chdir(super_case) != 0)
			{
				free(super_case);
				perror("chdir: ");
				return (1);
			}
			free(super_case);
		}
		else if (chdir(line[1]) != 0)
		{
			ft_putstr_fd("minishell: cd: no such file or directory: ", 2);
			ft_putstr_fd(line[1], 2);
			ft_putstr_fd("\n", 2);
			perror("chdir: ");
			return (1);
		}
		// free(super_case);
		reset_old_pwd(e_list);
		reset_pwd(e_list);
	}
	return (0);
}
