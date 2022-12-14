/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isdigit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 11:54:34 by lkavalia          #+#    #+#             */
/*   Updated: 2022/12/18 20:24:43 by lkavalia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_isdigit(int input)
{
	if (input >= '0' && input <= '9')
		return (1);
	else
		return (0);
}
