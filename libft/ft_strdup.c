/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbiederm <pbiederm@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 11:55:13 by lkavalia          #+#    #+#             */
/*   Updated: 2022/12/18 10:50:29 by pbiederm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
It duplicates the string
*/

char	*ft_strdup(const char *string)
{
	size_t	i;
	char	*a;

	i = 0;
	a = malloc((ft_strlen(string) + 1) * sizeof(char));
	if (!a)
		return (NULL);
	while (string[i] != '\0')
	{
		a[i] = string[i];
		i++;
	}
	a[i] = '\0';
	return (a);
}
