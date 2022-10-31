/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 11:54:39 by lkavalia          #+#    #+#             */
/*   Updated: 2022/10/31 12:04:08 by lkavalia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t i_counter(int n)
{
	int i;

	if (n == 0)
		return (1);
	i = 0;
	if (n < 0)
		i++;
	while (n != 0)
	{
		n = n / 10;
		i++;
	}
	return (i);
}

char *ft_itoa(int n)
{
	size_t len;
	size_t b;
	char *conv;

	len = i_counter(n);
	conv = malloc(sizeof(char) * (len + 1));
	if (!conv)
		return (NULL);
	conv[len] = '\0';
	if (n < 0)
	{
		conv[0] = '-';
		b = 1;
	}
	else
		b = 0;
	while (len-- > b)
	{
		if (n < 0)
			conv[len] = '0' + n % 10 * (-1);
		else
			conv[len] = '0' + n % 10;
		n = n / 10;
	}
	return (conv);
}
