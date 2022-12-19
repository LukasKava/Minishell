/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 12:37:27 by lkavalia          #+#    #+#             */
/*   Updated: 2022/12/19 14:07:38 by lkavalia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * FUNCTION: (ft_strcmp) takes compares s1 and s2 and returns
 * 				the according value.
 *	RETURN_VALUES:
 * 		:	0 When s1 and s2 are equal.
 * 		: > 0 if the first non-matching character in s1 is greater than that of s2.
 * 		: < 0 if the first non-matching character in s1 is less than that of s2.
 */
int	ft_strcmp(const char *s1, const char *s2)
{
	int	i;

	i = 0;
	while (s1[i] == s2[i] && (s1[i] != '\0' || s2[i] != '\0'))
		i++;
	return (s1[i] - s2[i]);
}

void	check_dollar_signs(t_info *info)
{
	int	i;

	i = 0;
	while (info->readline[i] != '\0')
	{
		if (info->readline[i] == '$' && info->readline[i + 1] == '$')
		{
			printf("ERROR (count_dollar_signs): dolar signs are together!\n");
			info->error = true;
			break ;
		}
		if (info->readline[i] == '$' && info->readline[i + 1] == '?')
			info->dollar_question_sign++;
		else if (info->readline[i] == '$' && info->readline[i + 1] != '?')
			info->dollar_sign++;
		i++;
	}
}

int	skip_white_sp(char *s, int i)
{
	while (s[i] == ' ' && s[i] != '\0')
		i++;
	return (i);
}

int	skip_quotes(char *str, char quote, int i)
{
	while (str[i] != quote && str[i] != '\0')
		i++;
	return (i);
}

char	*ft_delete(char *str, char *part)
{
	char	*new_str;
	int		i;
	int		len;

	i = 0;
	len = 0;
	new_str = NULL;
	while (part[i] != '\0')
		i++;
	len = ft_strlen(str) - i;
	new_str = ft_calloc(len + 1, sizeof(char));
	new_str[len] = '\0';
	len = 0;
	while (str[i] != '\0')
	{
		new_str[len] = str[i];
		i++;
		len++;
	}
	free(str);
	return (new_str);
}
