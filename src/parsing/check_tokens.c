/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 12:37:11 by lkavalia          #+#    #+#             */
/*   Updated: 2022/12/13 12:25:47 by lkavalia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static bool	error_message(t_info *info, int exit_s)
{
	printf("\033[0;31mSyntax error\033[0m\n");
	g_exit_status = exit_s;
	info->error = true;
	return (true);
}

/**
 * FUNCTION: (check_token) checks the possible token errors. For example:
 * 				('echo hello >') Because there is no output file.
 * 				('>> >>') Because one special token goes after another.
 * 				The list of the possible tokens:
 *		x ampersand ('&') = -6
 *		x double_ampersand ('&&') = -5
 *		x semicolon	(';') = -4
 *		x double_semicolon (';;') = -3
 *		x left_bracket ('(') = -2
 *		x right_bracket (')') = -1
 *		|space = 0
 *		|pipe = 1
 *		|redirection_input = 2
 *		|redirection_output = 3
 *		|redirect_append_input = 4
 *		|redirect_append_output = 5
 */
void	check_tokens(t_info *info, t_token **token)
{
	t_token	*temp;
	bool	error;

	temp = (*token);
	error = false;
	while ((*token) != NULL && error == false)
	{
		if (((*token)->name >= PIPE \
				&& (*token)->name <= R_AP_OUTPUT) \
				&& (*token)->next == NULL)
			error = error_message(info, 2);
		else if (((*token)->name >= PIPE \
				&& (*token)->name <= R_AP_OUTPUT) \
				&& (((*token)->next->name >= FORCED_R \
				&& (*token)->next->name < SPace) \
				|| ((*token)->next->name == PIPE)))
			error = error_message(info, 2);
		(*token) = (*token)->next;
	}
	(*token) = temp;
}
