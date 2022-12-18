/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbiederm <pbiederm@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 11:55:11 by lkavalia          #+#    #+#             */
/*   Updated: 2022/12/18 10:42:39 by pbiederm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
This function searches fot the first occurrence of the char c ir the
string pointed to by the argument str
*/

char	*ft_strchr(const char *str, int c)
{
	int		i;
	char	*b;

	i = 0;
	b = (char *)str;
	if (c > 256)
		c = c - 256;
	while (b[i] != '\0' && c != b[i])
		i++;
	if (b[i] == c)
		return ((char *)str + i);
	return (0);
}
