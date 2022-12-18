/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbiederm <pbiederm@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 11:55:34 by lkavalia          #+#    #+#             */
/*   Updated: 2022/12/18 10:38:44 by pbiederm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
This function searches for the last occurrence of the char in the string
pointed to, by the argument str.
*/

char	*ft_strrchr(const char *str, int c)
{
	int		i;
	int		b;
	char	*f_str;

	i = ft_strlen(str);
	b = 0;
	f_str = (char *)str;
	if (c > 256)
		c = c - 256;
	while (i >= b)
	{
		if (f_str[i] == c)
			return ((char *)str + i);
		else
			i--;
	}
	return (0);
}
