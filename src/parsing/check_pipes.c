/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_pipes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 12:37:05 by lkavalia          #+#    #+#             */
/*   Updated: 2022/12/23 04:47:53 by lkavalia         ###   ########.fr       */
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
	while (info->r[i] == ' ')
		i++;
	if (info->r[i] == '|')
		return (1);
	while (info->r[i])
	{
		if (info->r[i] == '"' || info->r[i] == 39)
			i = skip_quotes(info->r, info->r[i], i + 1);
		if (info->r[i] == '|' \
			&& info->r[skip_white_sp(info->r, i + 1)] == '|' )
			return (1);
		if (info->r[i] == '|')
			pipe = 1;
		if ((info->r[i] != ' ' && info->r[i] != '|') && pipe == 1)
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
	while (info->r[i] != '\0')
	{
		if (info->r[i] == 34 || info->r[i] == 39)
			i = skip_quotes(info->r, info->r[i], i + 1);
		else if (info->r[i] == '|')
			info->pipes++;
		i++;
	}
	if (info->pipes != 0)
		return (0);
	return (1);
}

int	check_for_spaces(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] != ' ')
			return (1);
		i++;
	}
	return (0);
}

int	pipe_redirections(t_token **t)
{
	if ((*t)->next->d_quotes == 0 && (*t)->next->s_quotes == 0 && \
		((*t)->name != PIPE && ((*t)->next->name <= PIPE && \
		(*t)->next->name > R_AP_OUTPUT)))
		return (0);
	return (1);
}
