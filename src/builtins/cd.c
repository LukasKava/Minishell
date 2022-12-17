/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbiederm <pbiederm@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 14:04:01 by lkavalia          #+#    #+#             */
/*   Updated: 2022/12/18 00:41:39 by pbiederm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	reset_old_pwd(t_env **e)
{
	t_env	*temp;
	char	*curr_pwd;

	temp = (*e);
	while ((*e) != NULL && (*e)->var != ft_strnstr((*e)->var, "PWD", 4))
		(*e) = (*e)->next;
	if ((*e) == NULL)
		return ;
	curr_pwd = (*e)->trim_var;
	(*e) = temp;
	while ((*e) != NULL && (*e)->var != ft_strnstr((*e)->var, "OLDPWD", 7))
		(*e) = (*e)->next;
	if ((*e) == NULL)
		return ;
	free((*e)->var);
	free((*e)->trim_var);
	(*e)->trim_var = ft_strdup(curr_pwd);
	(*e)->var = ft_strdup("OLDPWD=");
	(*e)->var = ft_strjoin((*e)->var, curr_pwd);
	(*e) = temp;
}

static void	reset_pwd(t_env **e)
{
	char	curr_dir[PATH_MAX];
	char	*pwd;
	t_env	*temp;

	pwd = NULL;
	temp = (*e);
	while ((*e) != NULL && (*e)->var != ft_strnstr((*e)->var, "PWD", 7))
		(*e) = (*e)->next;
	if ((*e) == NULL)
		return ;
	free((*e)->var);
	free((*e)->trim_var);
	pwd = getcwd(curr_dir, PATH_MAX);
	(*e)->var = ft_strdup("PWD=");
	(*e)->trim_var = ft_strdup(pwd);
	(*e)->var = ft_strjoin((*e)->var, pwd);
	(*e) = temp;
}

static int	cd_errors(int err_nbr)
{
	perror("chdir a: ");
	g_errors.g_exit_status = err_nbr;
	return (1);
}

static int	cd_possible(char **str, char *s_case, t_env **e_list, t_env **exp_l)
{
	if (check_s_c(str) != 0)
	{
		s_case = all_cases(e_list, str, check_s_c(str));
		if (chdir(s_case) != 0)
		{
			free(s_case);
			return (cd_errors(10));
		}
		free(s_case);
	}
	else if (chdir(str[1]) != 0)
		return (cd_errors(10));
	reset_old_pwd(e_list);
	reset_pwd(e_list);
	reset_old_pwd(exp_l);
	reset_pwd(exp_l);
	g_errors.g_exit_status = 0;
	return (0);
}

int	builtins_cd(char **line, t_env **e_list, t_env **exp_list)
{
	char	*super_case;

	super_case = NULL;
	if (!line[1])
	{
		if (chdir(get_home(e_list)) != 0)
			return (cd_errors(10));
		reset_old_pwd(e_list);
		reset_pwd(e_list);
		reset_old_pwd(exp_list);
		reset_pwd(exp_list);
	}
	else if (line[2] != NULL)
	{
		write(2, "\033[0;31mTo many arguments!\n\033[0m", 31);
		g_errors.g_exit_status = 10;
		return (1);
	}
	else
	{
		cd_possible(line, super_case, e_list, exp_list);
		return (g_errors.g_exit_status);
	}
	g_errors.g_exit_status = 0;
	return (0);
}
