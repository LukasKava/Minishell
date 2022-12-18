/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   register_tokens.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 12:37:24 by lkavalia          #+#    #+#             */
/*   Updated: 2022/12/18 19:59:29 by lkavalia         ###   ########.fr       */
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

static	int	check_for_spaces(char *str)
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
	if ((*token)->token[0] == '|')
		(*token)->name = PIPE;
	else if ((*token)->token[0] == '<' && (*token)->token[1] != '<')
		(*token)->name = R_INPUT;
	else if ((*token)->token[0] == '<' && (*token)->token[1] == '<')
		(*token)->name = R_AP_INPUT;
	else if ((*token)->token[0] == '>' && (*token)->token[1] != '>')
		(*token)->name = R_OUTPUT;
	else if ((*token)->token[0] == '>' && (*token)->token[1] == '>')
		(*token)->name = R_AP_OUTPUT;
	else if ((*token)->token[0] == '\0' && ((*token)->double_quotes == 1 || \
	(*token)->single_quotes == 1))
		(*token)->name = EMPTY;
	else if (check_for_spaces((*token)->token) == 0)
		(*token)->name = SPC;
	else if ((*token)->token[0] == '-' && ft_isalpha((*token)->token[1]) == 1)
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
		if ((*token)->ignore == true)
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

char	*ft_delete(char *str, char *part)
{
	char	*new_str;
	int		i;
	int		len;

	i = 0;
	len = 0;
	new_str = NULL;
	while (part[i] != '\0')
		i++;
	len = ft_strlen(str) - i;
	new_str = ft_calloc(len + 1, sizeof(char));
	new_str[len] = '\0';
	len = 0;
	while (str[i] != '\0')
	{
		new_str[len] = str[i];
		i++;
		len++;
	}
	free(str);
	return (new_str);
}

void	register_tokens(t_info *info, t_token **token, t_env *env)
{
	t_token	*temp_token;

	temp_token = (*token);
	expand_expansions(token, env);
	assign_indexes(token, info);
	connecting_quotes(token);
	if (info->error == false)
		check_tokens(info, token);
	if (info->error == false)
	{
		recognise_commands(token);
		check_command_excists(token, env);
		ignore(token);
	}
	(*token) = temp_token;
}
