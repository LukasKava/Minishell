/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/17 13:31:24 by lkavalia          #+#    #+#             */
/*   Updated: 2022/12/17 22:14:37 by lkavalia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"../../includes/minishell.h"

void	write_name(char *str, int fd)
{
	int	i;

	i = 0;
	while (str[i] != '\0' && str[i] != '=')
	{
		ft_putchar_fd(str[i], fd);
		i++;
	}
	if (str[i] != '\0')
	{
		ft_putstr_fd("=\"", fd);
		if (str[i] == '=')
		{
			i++;
			while (str[i] != '\0')
			{
				ft_putchar_fd(str[i], fd);
				i++;
			}
		}
		ft_putchar_fd(34, fd);
	}
	ft_putchar_fd('\n', fd);
}

void	print_export_l(t_env *ex_l, int fd)
{
	while (ex_l != NULL)
	{
		ft_putstr_fd("declare -x ", fd);
		write_name(ex_l->var, fd);
		ex_l = ex_l->next;
	}
}

void	swap_data(t_env **exl)
{
	char	*t_v;
	char	*t_v_n;

	t_v = ft_strdup((*exl)->var);
	t_v_n = ft_strdup((*exl)->var_name);
	free((*exl)->var);
	free((*exl)->var_name);
	(*exl)->var = ft_strdup((*exl)->next->var);
	(*exl)->var_name = ft_strdup((*exl)->next->var_name);
	free((*exl)->next->var);
	free((*exl)->next->var_name);
	(*exl)->next->var = ft_strdup(t_v);
	(*exl)->next->var_name = ft_strdup(t_v_n);
	free(t_v);
	free(t_v_n);
}

void	sort_indexes(t_env **exl)
{
	t_env	*temp;

	temp = (*exl);
	while ((*exl)->next != NULL)
	{
		if (ft_strncmp((*exl)->var_name, (*exl)->next->var_name, \
						check_len(*exl)) > 0)
		{
			swap_data((exl));
			(*exl) = temp;
		}
		else
			(*exl) = (*exl)->next;
	}
	(*exl) = temp;
}
