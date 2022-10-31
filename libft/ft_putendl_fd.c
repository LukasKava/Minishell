/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putendl_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 11:54:59 by lkavalia          #+#    #+#             */
/*   Updated: 2022/10/31 12:07:22 by lkavalia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void ft_putendl_fd(char *s, int fd)
{
	int i;

	i = 0;
	if (!s || fd < 0)
		return;
	else
	{
		while (s[i] != '\0')
			i++;
		write(fd, s, i);
		write(fd, "\n", 1);
	}
}
