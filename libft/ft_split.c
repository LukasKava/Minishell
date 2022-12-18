/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbiederm <pbiederm@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 11:55:08 by lkavalia          #+#    #+#             */
/*   Updated: 2022/12/18 10:41:28 by pbiederm         ###   ########.fr       */
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
	int		x;

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

// static size_t word_count(char const *s, char c)
// {
// 	size_t word_count;
// 	size_t i;
// 	size_t in_word;

// 	i = 0;
// 	in_word = 0;
// 	word_count = 0;
// 	while (s[i] != '\0')
// 	{
// 		if (s[i] != c)
// 			in_word = 1;
// 		if (in_word == 1 && s[i] == c)
// 		{
// 			in_word = 0;
// 			word_count++;
// 		}
// 		i++;
// 	}
// 	if (in_word == 1)
// 		word_count++;
// 	return (word_count);
// }

// static size_t word_start(char *s, char del, size_t pos)
// {
// 	if (s[pos] != del)
// 		return (pos);
// 	while (s[pos] == del)
// 	{
// 		pos++;
// 	}
// 	if (s[pos] != del)
// 		return (pos);
// 	else
// 		return (0);
// }

// static size_t word_len(char *s, char c, size_t pos)
// {
// 	size_t len;

// 	len = 0;
// 	while (s[pos] != '\0' && s[pos] != c)
// 	{
// 		len++;
// 		pos++;
// 	}
// 	return (len);
// }

// char **ft_split(char const *s, char c)
// {
// 	char **adr_arr;
// 	size_t w_count;
// 	size_t i;
// 	size_t start;
// 	size_t len;

// 	w_count = word_count(s, c);
// 	adr_arr = (char **)malloc((w_count + 1) * sizeof(char *));
// 	if (!adr_arr)
// 		return (NULL);
// 	i = 0;
// 	len = 0;
// 	start = 0;
// 	while (i < w_count)
// 	{
// 		start = word_start((char *)s, c, start + len);
// 		len = word_len((char *)s, c, start);
// 		adr_arr[i++] = ft_substr((char *)s, start, len);
// 	}
// 	adr_arr[i] = (NULL);
// 	return (adr_arr);
// }