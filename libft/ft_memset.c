/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 11:54:52 by lkavalia          #+#    #+#             */
/*   Updated: 2022/12/18 20:25:05 by lkavalia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
this function copies the char input  to the first n
characters of the string pointed t0, by the argument str.
*/

void	*ft_memset(void *str, int input, size_t n)
{
	size_t			i;
	unsigned char	*a;

	i = 0;
	a = (unsigned char *)str;
	while (i < n)
	{
		a[i] = input;
		i++;
	}
	return (str = a);
}
