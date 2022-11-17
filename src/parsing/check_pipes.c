/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_pipes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 12:37:05 by lkavalia          #+#    #+#             */
/*   Updated: 2022/11/17 04:00:54 by lkavalia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * FUCNTION: (pipe_cases) checks for the specific error cases on the pipes.
 * 				There are specific five cases that we are checking for:
 * case 1: // | = syntax error
 * case 2: // | echo = syntax error
 * case 3: // "|" = handled as a normal char value
 * case 4: // '|' = handles as normal char value.
 * case 5: // echo | = syntax error
 *
 * RETURN VALUE---------
 *	0	-In case the pipes that are currently in the input does not
 *			match these specific error cases.
 *	1	-In case in the given input the current pipe match any requirements
 *			function fails.
 */
int	pipe_cases(t_info *info)
{
	int	i;
	int	pipe;

	i = 0;
	pipe = 0;
	while (info->readline[i] == ' ')
		i++;
	if (info->readline[i] == '|')
		return (1);
	while (info->readline[i])
	{
		if (info->readline[i] == '"' || info->readline[i] == 39)
			i = skip_quotes(info->readline, info->readline[i], i + 1);
		if (info->readline[i] == '|' \
			&& info->readline[skip_white_sp(info->readline, i + 1)] == '|' )
			return (1);
		if (info->readline[i] == '|')
			pipe = 1;
		if ((info->readline[i] != ' ' && info->readline[i] != '|') && pipe == 1)
			pipe = 0;
		i++;
	}
	if (pipe == 1)
		return (1);
	return (0);
}

/**
 * FUNCTION: (check_pipes) checks if there are
 * 				any pipes in the input.
 * RETURN VALUE------
 * 	0 -if it has succesfully found a pipe no matter if it is
 * 		sintaxicly possible or not.
 *	1 -if there are no pipes in a given input.
 */
int	pipe_excistence(t_info *info)
{
	int	i;

	i = 0;
	while (info->readline[i] != '\0')
	{
		if (info->readline[i] == 34 || info->readline[i] == 39)
			i = skip_quotes(info->readline, info->readline[i], i + 1);
		else if (info->readline[i] == '|')
			info->pipes++;
		i++;
	}
	if (info->pipes != 0)
		return (0);
	return (1);
}
