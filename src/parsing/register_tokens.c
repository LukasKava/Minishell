/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   register_tokens.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 12:37:24 by lkavalia          #+#    #+#             */
/*   Updated: 2022/12/21 03:25:36 by lkavalia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 *	x everything else = -9
 *	x forced redirection ('>|') = -7	its not manditory.
 *	x ampersand ('&') = -6				its not manditory
 *	x double_ampersand ('&&') = -5		its not manditory
 *	x semicolon	(';') = -4				its not manditory
 *	x double_semicolon (';;') = -3		its not manditory
 *	x left_bracket ('(') = -2			its not manditory
 *	x right_bracket (')') = -1			its not manditory
 *	|space = 0
 *	|pipe = 1
 *	|redirection_input = 2
 *	|redirection_output = 3
 *	|redirect_append_input = 4
 *	|redirect_append_output = 5
 *	|escape_char '\' == 6
 *	command = 7
 *	built-in = 8
 *	arguments = 9
 *	flags = 10
 *	input_file = 11
 *	output_file = 12
 *	delimitor = 13
 */

/**
 * FUNCTION: (check_specials_outside) checks for special character combitions
 * 				that are excluded by the by mandatory but might be in the bonus.
 *	x forced redirection ('>|') = -7	its not manditory.
 *	x ampersand ('&') = -6				its not manditory.
 *	x double_ampersand ('&&') = -5		its not manditory.
 *	x semicolon	(';') = -4				its not manditory.
 *	x double_semicolon (';;') = -3		its not manditory.
 *	x left_bracket ('(') = -2			its not manditory.
 *	x right_bracket (')') = -1			its not manditory.
 *	x escape_char	('\') = 6			its not manditory.
 */
/* static bool specials_outside(t_token **token)
{
	if ((*token)->token[0] == '>' && (*token)->token[1] == '|')
		message(token, FORCED_R);
	else if ((*token)->token[0] == '&' && (*token)->token[1] != '&')
		message(token, AMPERSAND);
	else if ((*token)->token[0] == '&' && (*token)->token[1] == '&')
		message(token, DOUBLE_AMPERSAND);
	else if ((*token)->token[0] == ';' && (*token)->token[1] != ';')
		message(token, SEMICOLON);
	else if ((*token)->token[0] == ';' && (*token)->token[1] == ';')
		message(token, DOUBLE_SEMICOLON);
	else if ((*token)->token[0] == '(')
		message(token, L_BRACKET);
	else if ((*token)->token[0] == ')')
		message(token, R_BRACKET);
	else if ((*token)->token[0] == '\\')
		message(token, ESCAPE);
	if ((*token)->ignore == true)
		return (true);
	return (false);
} */

/**
 * FUNCTION: (check_speacials) checks for special character combinations
 * 				that are included in mandatory part of the minishell.
 *		|pipe = 1
 *		|redirection_input = 2
 *		|redirection_output = 3
 *		|redirect_append_input = 4
 *		|redirect_append_output = 5
 */
static void	check_specials(t_token **token)
{
	if ((*token)->s_quotes == true || (*token)->d_quotes == true)
		(*token)->name = ELSE;
	else if ((*token)->t[0] == '|')
		(*token)->name = PIPE;
	else if ((*token)->t[0] == '<' && (*token)->t[1] != '<')
		(*token)->name = R_INPUT;
	else if ((*token)->t[0] == '<' && (*token)->t[1] == '<')
		(*token)->name = R_AP_INPUT;
	else if ((*token)->t[0] == '>' && (*token)->t[1] != '>')
		(*token)->name = R_OUTPUT;
	else if ((*token)->t[0] == '>' && (*token)->t[1] == '>')
		(*token)->name = R_AP_OUTPUT;
	else if ((*token)->t[0] == '\0' && ((*token)->d_quotes == 1 || \
										(*token)->s_quotes == 1))
		(*token)->name = EMPTY;
	else if (check_for_spaces((*token)->t) == 0)
		(*token)->name = SPC;
	else if ((*token)->t[0] == '-' && ft_isalpha((*token)->t[1]) == 1)
		(*token)->name = FLAG;
	else
		(*token)->name = ELSE;
}

/**
 * FUNCTION: (assign_indexes) goes through the list and assings index values
 * 				to the token. While doing that it checks for special characters and
 * 				also checks if the token is a builtin or not.
 */
static void	assign_indexes(t_token **token, t_info *info)
{
	t_token	*temp;
	int		i;

	temp = (*token);
	i = 0;
	while ((*token) != NULL)
	{
		check_specials(token);
		recognise_builtins(token);
		if ((*token)->ignore == true && ((*token)->name != SPC && \
			(*token)->name != EMPTY))
		{
			info->error = true;
			g_errors.g_exit_status = 2;
			break ;
		}
		(*token)->index = i;
		(*token) = (*token)->next;
		i++;
	}
	(*token) = temp;
}

/* static void	connecting_spaces(t_token **token)
{
	t_token	*temp;

	temp = (*token);
	while ((*token) != NULL)
	{
		if ((*token)->next != NULL)
		{
			if (space_check(*token) == -1 && ((*token)->s_quotes == true
	 || (*token)->d_quotes == true))
			{
				printf("next: [%s]\n", (*token)->next->t);
				printf("next_name: [%d]\n", (*token)->next->name);
				if ((*token)->next->name != PIPE)
				{
					if ((*token)->next->ignore == true &&
					 (*token)->next->d_quotes != NULL && 
						)
					{
						printf("BOMB\n");
						(*token)->s_quotes = true;
						(*token)->d_quotes = true;
					}
				}
			}
		}
		(*token) = (*token)->next;
	}
	(*token) = temp;
} */

static void	delete_ignored_spaces(t_token **token)
{
	t_token	*temp;
	t_token	*delete;
	int		ignore;

	temp = (*token);
	delete = NULL;
	ignore = 0;
	while ((*token) != NULL)
	{
		if ((*token)->next != NULL)
		{
			if ((*token)->name == SPC && (*token)->ignore == true && \
				((*token)->s_quotes == true || (*token)->d_quotes == true))
			{
				ignore = change_the_node(token, temp, delete, ignore);
			}
		}
		if (ignore != 1)
			(*token) = (*token)->next;
		ignore = 0;
	}
	(*token) = temp;
}

static void	set_up_ignores(t_token **t)
{
	t_token	*temp;

	temp = (*t);
	while ((*t) != NULL)
	{
		if ((*t)->next != NULL)
		{
			if (space_check(*t) == -1 && \
				((*t)->s_quotes == true || (*t)->d_quotes == true))
			{
				if ((*t)->next->ignore != true && \
					space_check((*t)->next) == -1)
					(*t)->ignore = true;
			}
		}
		(*t) = (*t)->next;
	}
	(*t) = temp;
}

void	register_tokens(t_info *info, t_token **token, t_env *env)
{
	t_token	*temp_token;

	temp_token = (*token);
	expand_expansions(token, env);
	set_up_ignores(token);
	delete_ignored_spaces(token);
	check_for_deleting_spaces(token);
	delete_ignored_spaces(token);
	connecting_quotes(token);
	assign_indexes(token, info);
	if (info->error == false)
		check_tokens(info, token);
	//print_the_list("check_tokens!", *token);
	if (info->error == false)
	{
		recognise_commands(token);
		//print_the_list("recognise_commands!", *token);
		check_command_excists(token, env);
		//print_the_list("check_commands_excist!", *token);
		ignore(token);
		//print_the_list("ignore!", *token);
	}
	(*token) = temp_token;
}
