/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 11:55:26 by lkavalia          #+#    #+#             */
/*   Updated: 2022/10/31 12:11:04 by lkavalia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
size_t is used for array indexing and counting.
It can not ever be negative.
*/

size_t ft_strlen(const char *c)
{
	int i;

	i = 0;
	while (c[i] != '\0')
		i++;
	return (i);
}
