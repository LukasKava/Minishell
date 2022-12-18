/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 11:55:08 by lkavalia          #+#    #+#             */
/*   Updated: 2022/12/18 20:25:31 by lkavalia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
Allocates with malloc and returns an array of strings obtained by splitting
s  using the char c.
*/

#include "libft.h"

/**
 * ft_split.c is a file containing a split
 * function (including its helper functions)from the libft.
 * It is reused this way to avoid using all of the Libft library.
 */

int	ft_word_count(const char *s1, char c)
{
	int	w_count;
	int	i;

	i = 0;
	w_count = 0;
	while (s1[i] != '\0')
	{
		if (c == s1[i])
			i++;
		else
		{
			w_count++;
			while (s1[i] && s1[i] != c)
				i++;
		}
	}
	return (w_count);
}

static char	*ft_insert(char *word, char const *s1, size_t i, size_t w_len)
{
	int	x;

	x = 0;
	while (w_len > 0)
	{
		word[x] = s1[i - w_len];
		w_len--;
		x++;
	}
	word[x] = '\0';
	return (word);
}

static char	**s_words(size_t w_count, char const *s1, char c, char **splitted)
{
	size_t	x;
	size_t	i;
	size_t	w_len;

	x = 0;
	i = 0;
	w_len = 0;
	while (x < w_count)
	{
		while (s1[i] == c && s1[i] != '\0')
			i++;
		while (s1[i] != c && s1[i] != '\0')
		{
			i++;
			w_len++;
		}
		splitted[x] = malloc(sizeof(char) * (w_len + 1));
		if (!splitted)
			return (NULL);
		ft_insert(splitted[x], s1, i, w_len);
		w_len = 0;
		x++;
	}
	splitted[x] = 0;
	return (splitted);
}

char	**ft_split(char const *s1, char c)
{
	size_t	w_count;
	char	**splitted;

	if (!s1)
		return (NULL);
	w_count = ft_word_count(s1, c);
	splitted = malloc(sizeof(char *) * (w_count + 1));
	if (!splitted)
		return (NULL);
	s_words(w_count, s1, c, splitted);
	return (splitted);
}
