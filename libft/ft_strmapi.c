/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 11:55:29 by lkavalia          #+#    #+#             */
/*   Updated: 2022/10/31 12:11:27 by lkavalia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
Apllies the function f to each char of the string s and it
returns a new string with those changes
*/

char *ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char *ans;
	int i;

	ans = ft_strdup(s);
	i = 0;
	if (!ans || !(*f))
		return (NULL);
	else
	{
		while (ans[i] != '\0')
		{
			ans[i] = (*f)(i, ans[i]);
			i++;
		}
	}
	return (ans);
}
