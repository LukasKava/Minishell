/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 12:37:11 by lkavalia          #+#    #+#             */
/*   Updated: 2022/11/10 12:51:36 by lkavalia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
void check_tokens(t_info *info, t_token **token)
{
	t_token *temp;

	temp = (*token);
	while ((*token) != NULL)
	{
		if (((*token)->name >= PIPE && (*token)->name <= R_AP_OUTPUT) && (*token)->next == NULL)
		{
			printf("bash: syntax error near unexpected token `newline'\n");
			info->exit_status = 2;
			info->error = true;
			break;
		}
		else if (((*token)->name >= PIPE && (*token)->name <= R_AP_OUTPUT) && (((*token)->next->name >= FORCED_R && (*token)->next->name < SPace) || ((*token)->next->name >SPace && (*token)->next->name <= R_AP_OUTPUT)))
		{
			printf("bash: syntax error near unexpected token `%s'\n", (*token)->next->token);
			info->exit_status = 2;
			info->error = true;
			break;
		}
		(*token) = (*token)->next;
	}
	(*token) = temp;
}
