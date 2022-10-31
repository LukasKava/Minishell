/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 12:37:08 by lkavalia          #+#    #+#             */
/*   Updated: 2022/10/31 12:42:10 by lkavalia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int check_quotes(t_info *info)
{
	size_t i;
	char quote;

	i = 0;
	quote = '1';
	while (info->readline[i] != '\0')
	{
		if (info->readline[i] == '"' || info->readline[i] == 39)
		{
			quote = info->readline[i];
			i++;
			while (info->readline[i] != quote && info->readline[i] != '\0')
				i++;
			if (info->readline[i] != quote)
				return (1);
		}
		i++;
	}
	return (0);
}