/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 11:54:47 by lkavalia          #+#    #+#             */
/*   Updated: 2022/12/18 20:24:59 by lkavalia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
This function copies n characters from the memory area loc to dest.
"const" variables can only be initialized once.
*/

void	*ft_memcpy(void *dest, const void *loc, size_t n)
{
	size_t			i;
	unsigned char	*a;
	unsigned char	*b;

	i = 0;
	a = (unsigned char *)dest;
	b = (unsigned char *)loc;
	if ((dest == loc) || n == 0)
		return (dest);
	if (!dest && !loc)
		return (0);
	while (i < n)
	{
		a[i] = b[i];
		i++;
	}
	return (dest);
}
