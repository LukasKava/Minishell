/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 11:55:06 by lkavalia          #+#    #+#             */
/*   Updated: 2022/10/31 12:08:16 by lkavalia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void ft_putstr_fd(char *s, int fd)
{
	int i;

	i = 0;
	if (fd < 0 || !s)
		return;
	else
	{
		while (s[i] != '\0')
			i++;
		write(fd, s, i);
	}
}
