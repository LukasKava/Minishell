/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 03:26:55 by lkavalia          #+#    #+#             */
/*   Updated: 2022/12/06 14:32:20 by lkavalia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*ft_strtrim_beginning(char *s1, char *s2)
{
	int		b;
	int		len;
	char	*final;

	b = 0;
	len = 0;
	final = NULL;
	while (s1[b] != '\0' && s2[b] != '\0' && s1[b] == s2[b])
		b++;
	while (s1[b++] != '\0')
		len++;
	len++;
	final = ft_calloc(len + 1, sizeof(char));
	final[len] = '\0';
	len--;
	b--;
	while (len >= 0)
		final[len--] = s1[b--];
	return (final);
}

char	*save_the_front(char *token)
{
	char	*front;
	int		len;

	len = 0;
	front = NULL;
	while (token[len] != '\0' && token[len] != '$')
		len++;
	front = ft_calloc(len + 1, sizeof(char));
	front[len] = '\0';
	len--;
	while (len >= 0)
	{
		front[len] = token[len];
		len--;
	}
	return (front);
}

char	*save_tail(char *str)
{
	char *tail;
	int i;
	int len;

	tail = NULL;
	i = 0;
	len = 0;
	while (str[i] != '\0' && str[i] != '$')
		i++;
	i++;
	while (str[i] != '\0' && ((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z') || (str[i] >= '0' && str[i] <= '9') || str[i] == '_'))
		i++;
	while (str[i] != '\0')
	{
		len++;
		i++;
	}
	tail = ft_calloc(len + 1, sizeof(char));
	tail[len] = '\0';
	len--;
	i--;
	while (len >= 0)
		tail[len--] = str[i--];
	return (tail);
}

int confirm_expansion(t_token *token)
{
	int i;

	i = 0;
	while (token->token[i] != '\0')
	{
		if (token->token[i] == '$')
		{
			i++;
			if (token->token[i] == '\0')
				return (1);
			if ((token->token[i] >= 'a' && token->token[i] <= 'z') ||
				(token->token[i] >= 'A' && token->token[i] <= 'Z') ||
				(token->token[i] >= '0' && token->token[i] <= '9') ||
				token->token[i] == '_')
				return (0);
		}
		i++;
	}
	return (1);
}
