/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbiederm <pbiederm@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/13 16:44:40 by pbiederm          #+#    #+#             */
/*   Updated: 2022/05/24 08:50:04 by pbiederm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	word_count(char const *s, char c)
{
	size_t	word_count;
	size_t	i;
	size_t	in_word;

	i = 0;
	in_word = 0;
	word_count = 0;
	while (s[i] != '\0')
	{
		if (s[i] != c)
			in_word = 1;
		if (in_word == 1 && s[i] == c)
		{
			in_word = 0;
			word_count++;
		}	
	i++;
	}
	if (in_word == 1)
		word_count++;
	return (word_count);
}

static size_t	word_start(char *s, char del, size_t pos)
{
	if (s[pos] != del)
		return (pos);
	while (s[pos] == del)
	{
		pos++;
	}
	if (s[pos] != del)
		return (pos);
	else
		return (0);
}

static size_t	word_len(char *s, char c, size_t pos)
{
	size_t	len;

	len = 0;
	while (s[pos] != '\0' && s[pos] != c)
	{
		len++;
		pos++;
	}
	return (len);
}

char	**ft_split(char const *s, char c)
{
	char	**adr_arr;
	size_t	w_count;
	size_t	i;
	size_t	start;
	size_t	len;

	w_count = word_count(s, c);
	adr_arr = (char **)malloc((w_count + 1) * sizeof(char *));
	if (!adr_arr)
		return (NULL);
	i = 0;
	len = 0;
	start = 0;
	while (i < w_count)
	{
		start = word_start((char *)s, c, start + len);
		len = word_len((char *)s, c, start);
		adr_arr[i++] = ft_substr((char *)s, start, len);
	}
	adr_arr[i] = (NULL);
	return (adr_arr);
}
// int main (void)
// {
// 	char	**adr_arrr;
// 	size_t	n;

// 	adr_arrr = ft_split("--1-2--3---4----5-----42", '-');
// 	n = 0;
// while (n <= 6)
// {
// 	printf ("word_count: %s\n",adr_arrr[n]);
// 	n++;
// }
// 	free(adr_arrr);
// 	return (0);
// }
