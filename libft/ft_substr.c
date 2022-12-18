/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 11:55:42 by lkavalia          #+#    #+#             */
/*   Updated: 2022/12/18 20:13:50 by lkavalia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
Allocates  and returns a substring from the string s.
The substring begins at the index start and is of the max size len.
*/

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*real_s;
	size_t	s_l;

	s_l = ft_strlen(s);
	if (!s)
		return (NULL);
	if (s_l < start)
	{
		free((char *)s);
		return (ft_strdup(""));
	}
	if (len > s_l - start)
		len = s_l - start;
	real_s = malloc((len + 1) * sizeof(char));
	if (!real_s)
	{
		free((char *)s);
		return (NULL);
	}
	ft_strlcpy(real_s, &s[start], len + 1);
	return (real_s);
}
