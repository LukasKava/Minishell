/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 12:37:27 by lkavalia          #+#    #+#             */
/*   Updated: 2022/10/31 12:44:45 by lkavalia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * FUNCTION (count_quotes) counts the quotes that are in the
 * 				input.
 */
void count_quotes(t_info *info)
{
	int i;
	int double_quotes;
	int single_quotes;

	i = 0;
	double_quotes = 0;
	single_quotes = 0;
	while (info->readline[i] != '\0')
	{
		if (info->readline[i] == 34)
			double_quotes++;
		else if (info->readline[i] == 39)
			single_quotes++;
		i++;
	}
	info->d_quotes = double_quotes;
	info->s_quotes = single_quotes;
}

/**
 * FUNCTION: (ft_strcmp) takes compares s1 and s2 and returns
 * 				the according value.
 *	RETURN_VALUES:
 * 		:	0 When s1 and s2 are equal.
 * 		: > 0 if the first non-matching character in s1 is greater than that of s2.
 * 		: < 0 if the first non-matching character in s1 is less than that of s2.
 */
int ft_strcmp(const char *s1, const char *s2)
{
	int i;

	i = 0;
	while (s1[i] == s2[i] && (s1[i] != '\0' || s2[i] != '\0'))
		i++;
	return (s1[i] - s2[i]);
}

// void count_redirections(t_info *info)
// {
// 	int	i;

// 	i = 0;
// 	while (info->readline[i] != '\0')
// 	{
// 		if (info->readline[i] == '<' && info->readline[i + 1] != '<' && info->readline[i - 1] != '<')
// 			info->redirect_input++;
// 		else if (info->readline[i] == '<' && info->readline[i + 1] == '<')
// 			info->redirect_input_append++;
// 		if (info->readline[i] == '>' && info->readline[i + 1] != '>' && info->readline[i - 1] != '>')
// 			info->redirect_output++;
// 		else if (info->readline[i] == '>' && info->readline[i + 1] == '>')
// 			info->redirect_output_append++;
// 		i++;
// 	}
// }

void check_dollar_signs(t_info *info)
{
	int i;

	i = 0;
	while (info->readline[i] != '\0')
	{
		if (info->readline[i] == '$' && info->readline[i + 1] == '$')
		{
			printf("ERROR (count_dollar_signs): dolar signs are together!\n");
			info->error = true;
			break;
		}
		if (info->readline[i] == '$' && info->readline[i + 1] == '?')
			info->dollar_question_sign++;
		else if (info->readline[i] == '$' && info->readline[i + 1] != '?')
			info->dollar_sign++;
		i++;
	}
}

int skip_white_sp(char *s, int i)
{
	while (s[i] == ' ' && s[i] != '\0')
		i++;
	return (i);
}
