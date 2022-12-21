/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 12:37:11 by lkavalia          #+#    #+#             */
/*   Updated: 2022/12/21 03:28:40 by lkavalia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static bool	err_m(t_info *info, char *message, int exit_s)
{
	printf("\033[0;31m%s!\033[0m\n", message);
	g_errors.g_exit_status = exit_s;
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
void	check_tokens(t_info *i, t_token **t)
{
	t_token	*temp;
	bool	err;

	temp = (*t);
	err = false;
	while ((*t) != NULL && err == false)
	{
		if (((*t)->s_quotes == false && (*t)->d_quotes == false))
		{
			if (((*t)->name >= PIPE && (*t)->name <= R_AP_OUTPUT) \
									&& (*t)->next == NULL)
				err = err_m(i, "Syntax err near unexpected token `newline'", 2);
			else if (((*t)->name >= PIPE && (*t)->name <= R_AP_OUTPUT))
			{
				if ((*t)->next->d_quotes == 0 && (*t)->next->s_quotes == 0)
				{
					if ((*t)->next->name >= PIPE && \
						(*t)->next->name <= R_AP_OUTPUT)
						err = err_m(i, "Syntax err near unexpected token", 2);
				}
			}
		}
		(*t) = (*t)->next;
	}
	(*t) = temp;
}

int	space_norm(t_token **token, t_info *info, int i)
{
	//printf("space function!\n");
	//printf ("i: %d: [%c]\n", i, info->r[i]);
	//printf("i - 1: %d: [%c]\n", i - 1, info->r[i - 1]);
	if (i - 1 != 0 && (info->r[i - 1] == 34 || info->r[i - 1] == 39))
		(*token)->ignore = true;
	i = space(info, i, token);
	//printf("after space: [%s] ignore: %d\n", (*token)->t, (*token)->ignore);
	return (i);
}

int	space_check(t_token *token)
{
	int	i;

	i = 0;
	if (token->t[0] == '\0')
		return (-1);
	while (token->t[i] != '\0')
	{
		if (token->t[i] != ' ')
			return (1);
		i++;
	}
	return (0);
}

void	first_token(t_token **t)
{
	if (space_check(*t) == -1 && \
		((*t)->s_quotes == true || (*t)->d_quotes == true))
	{
		if ((*t)->next != NULL)
		{
			if (space_check((*t)->next) != -1 && \
				(*t)->next->ignore == false)
				(*t)->ignore = true;
		}
	}
}
