/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 03:26:55 by lkavalia          #+#    #+#             */
/*   Updated: 2022/11/17 03:56:37 by lkavalia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

size_t	ft_case(char c)
{
	if (c >= 'a' && c <= 'z')
		return (0);
	else if (c >= 'A' && c <= 'Z')
		return (1);
	return (2);
}

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

void	ft_cut_exp(t_token **token)
{
	int		i;
	char	*tail;
	char	*front;
	int		tail_len;

	i = 0;
	tail = NULL;
	tail_len = 0;
	front = save_the_front((*token)->token);
	while ((*token)->token[i] != '\0' && (*token)->token[i] != '$')
		i++;
	i++;
	while ((*token)->token[i] != '\0' \
				&& (*token)->token[i] != '$' && (*token)->token[i] != ' ')
		i++;
	while ((*token)->token[i++] != '\0')
		tail_len++;
	tail = ft_calloc(tail_len + 1, sizeof(char));
	tail[tail_len] = '\0';
	i--;
	while (tail_len >= 0)
		tail[tail_len--] = (*token)->token[i--];
	free((*token)->token);
	(*token)->token = ft_strjoin(front, tail);
	free(tail);
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

char	*save_the_tail(char *token, char *current)
{
	char	*tail;
	char	*front;

	front = ft_strjoin(save_the_front(token), "$");
	front = ft_strjoin(front, current);
	tail = ft_strtrim_beginning(token, front);
	free(front);
	return (tail);
}
