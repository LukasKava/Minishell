/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 11:55:16 by lkavalia          #+#    #+#             */
/*   Updated: 2022/10/31 12:09:43 by lkavalia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void ft_striteri(char *s, void (*f)(unsigned int, char *))
{
	int i;

	i = 0;
	if (!s || !(*f))
		return;
	while (s[i] != '\0')
	{
		(*f)(i, &s[i]);
		i++;
	}
}
