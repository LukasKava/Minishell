/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   register_tokens.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 12:37:24 by lkavalia          #+#    #+#             */
/*   Updated: 2022/10/31 13:59:35 by lkavalia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 *	x everything else = -9
 *	x forced redirection ('>|') = -7	//Cast an error because its not manditory.
 *	x ampersand ('&') = -6				//Cast an error because its not manditory
 *	x double_ampersand ('&&') = -5		//Cast an error because its not manditory
 *	x semicolon	(';') = -4				//Cast an error because its not manditory
 *	x double_semicolon (';;') = -3		//Cast an error because its not manditory
 *	x left_bracket ('(') = -2			//Cast an error because its not manditory
 *	x right_bracket (')') = -1			//Cast an error because its not manditory
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

// static void message(t_token **token, int indentifier)
// {
// 	(*token)->ignore = true;
// 	(*token)->name = indentifier;
// 	printf("minishell: error unrecognised token: `%s'\n", (*token)->token);
// }

/**
 * FUNCTION: (check_specials_outside) checks for special character combitions
 * 				that are excluded by the by mandatory but might be in the bonus.
 *	x forced redirection ('>|') = -7	//Cast an error because its not manditory.
 *	x ampersand ('&') = -6				//Cast an error because its not manditory.
 *	x double_ampersand ('&&') = -5		//Cast an error because its not manditory.
 *	x semicolon	(';') = -4				//Cast an error because its not manditory.
 *	x double_semicolon (';;') = -3		//Cast an error because its not manditory.
 *	x left_bracket ('(') = -2			//Cast an error because its not manditory.
 *	x right_bracket (')') = -1			//Cast an error because its not manditory.
 *	x escape_char	('\') = 6			//Cast an error because its not manditory.
 */
// static bool specials_outside(t_token **token)
// {
// 	if ((*token)->token[0] == '>' && (*token)->token[1] == '|')
// 		message(token, FORCED_R);
// 	else if ((*token)->token[0] == '&' && (*token)->token[1] != '&')
// 		message(token, AMPERSAND);
// 	else if ((*token)->token[0] == '&' && (*token)->token[1] == '&')
// 		message(token, DOUBLE_AMPERSAND);
// 	else if ((*token)->token[0] == ';' && (*token)->token[1] != ';')
// 		message(token, SEMICOLON);
// 	else if ((*token)->token[0] == ';' && (*token)->token[1] == ';')
// 		message(token, DOUBLE_SEMICOLON);
// 	else if ((*token)->token[0] == '(')
// 		message(token, L_BRACKET);
// 	else if ((*token)->token[0] == ')')
// 		message(token, R_BRACKET);
// 	else if ((*token)->token[0] == '\\')
// 		message(token, ESCAPE);
// 	if ((*token)->ignore == true)
// 		return (true);
// 	return (false);
// }

static	int	check_for_spaces(char *str)
{
	int i;

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
static void check_specials(t_token **token)
{
	// if (specials_outside(token) == true)
	// 	return;
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
	else if ((*token)->token[0] == '\\')
		(*token)->name = ESCAPE;
	else if ((*token)->token[0] == '\0')
		(*token)->name = SPace;
	else if (check_for_spaces((*token)->token) == 0)
		(*token)->name = SPace;
	else if ((*token)->token[0] == '-' && ft_isalpha((*token)->token[1]) == 1)
		(*token)->name = FLAG;
	else
		(*token)->name = ELSE;
}

/**
 * FUNCTION: (recognise_builtins)	checks for the availible builtins and marks
 * 				the token if it is a possible builtin. Possibilities are:
 * 		echo		*	unset
 * 		cd			*	env
 * 		pwd			*	exit
 * 		export
 */
void recognise_builtins(t_token **token)
{
	int check;

	check = 0;
	if (ft_strcmp((*token)->token, "echo") == 0)
		check = BUILT_IN;
	else if (ft_strcmp((*token)->token, "cd") == 0)
		check = BUILT_IN;
	else if (ft_strcmp((*token)->token, "pwd") == 0)
		check = BUILT_IN;
	else if (ft_strcmp((*token)->token, "export") == 0)
		check = BUILT_IN;
	else if (ft_strcmp((*token)->token, "unset") == 0)
		check = BUILT_IN;
	else if (ft_strcmp((*token)->token, "env") == 0)
		check = BUILT_IN;
	else if (ft_strcmp((*token)->token, "exit") == 0)
		check = BUILT_IN;
	if (check == BUILT_IN)
		(*token)->name = BUILT_IN;
	else if ((*token)->name == BUILT_IN && check == 0)
		(*token)->name = -9;
}

/**
 * FUNCTION: (assign_indexes) goes through the list and assings index values
 * 				to the token. While doing that it checks for special characters and
 * 				also checks if the token is a builtin or not.
 */
static void assign_indexes(t_token **token, t_info *info)
{
	t_token *temp;
	int i;

	temp = (*token);
	i = 0;
	while ((*token) != NULL)
	{
		check_specials(token);
		recognise_builtins(token);
		if ((*token)->ignore == true)
		{
			info->error = true;
			g_exit_status = 2;
			break;
		}
		(*token)->index = i;
		(*token) = (*token)->next;
		i++;
	}
	(*token) = temp;
}

/**
 * FUNCTION: (register_next_token) makes sure that in case of redirections or
 * 				heredoc the next token would get assingned the corect name:
 * 		redirect input ('<') name 2 -> next_name = 11 input file
 * 		redirect output ('>') name 3 -> next_name = 12 output file
 *		Here document ('<<') name 4 -> next_name = 13 delimitor
 *		Redirect output append ('>>') inden.. 5 -> next_name = 12 output file
 */
static void register_next_token(t_token **token)
{
	if ((*token)->name == R_INPUT)
	{
		if ((*token)->next->name == SPace)
			(*token)->next->next->name = INPUT_F;
		else
			(*token)->next->name = INPUT_F;
	}
	else if ((*token)->name == R_OUTPUT)
	{
		if ((*token)->next->name == SPace)
			(*token)->next->next->name = OUTPUT_F;
		else
			(*token)->next->name = OUTPUT_F;
	}
	else if ((*token)->name == R_AP_INPUT)
	{
		if ((*token)->next->name == SPace)
			(*token)->next->next->name = DELIMITOR;
		else
			(*token)->next->name = DELIMITOR;
	}
	else if ((*token)->name == R_AP_OUTPUT)
	{
		if ((*token)->next->name == SPace)
			(*token)->next->next->name = R_AP_OUTPUT_F;
		else
			(*token)->next->name = R_AP_OUTPUT_F;
	}
}

static void recognise_commands(t_token **token)
{
	t_token *temp;

	temp = (*token);
	while ((*token) != NULL)
	{
		if ((*token)->name == BUILT_IN || (*token)->name == ELSE)
		{
			if ((*token)->name == ELSE)
				(*token)->name = COMMAND;
			(*token) = (*token)->next;
			while ((*token) != NULL && ((*token)->name == ELSE || \
			(*token)->name == SPace || (*token)->name > ESCAPE || (*token)->ignore == true))
			{
				if ((*token)->name == ELSE)
					(*token)->name = ARGUMENT;
				(*token) = (*token)->next;
			}
			if ((*token) == NULL)
				break ;
		}
		if ((*token)->name >= PIPE && (*token)->name <= R_AP_OUTPUT)
			register_next_token(token);
		(*token) = (*token)->next;
	}
	(*token) = temp;
}

char *ft_delete(char *str, char *part)
{
	char *new_str;
	int	i;
	int	len;

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

static void check_command_excists(t_token **token, t_env *env)
{
	int i;
	char *path;
	char **splitted_path;
	t_token *temp;

	i = 0;
	path = NULL;
	splitted_path = NULL;
	while (env != NULL && env->var != ft_strnstr(env->var, "PATH", 5))
		env = env->next;
	if (env == NULL)
	{
		printf("ERRROR in find path path is not excistent!\n");
		g_exit_status = 127;
		return ;
	}
	else
		path = env->var;
	temp = (*token);
	i = 0;
	while ((*token) != NULL)
	{
		if ((*token)->name == COMMAND)
		{
			splitted_path = ft_split(path, ':');
			splitted_path[0] = ft_delete(splitted_path[0], "PATH=");
			while (splitted_path[i] != NULL)
			{
				splitted_path[i] = ft_strjoin(splitted_path[i], "/");
				splitted_path[i] = ft_strjoin(splitted_path[i], (*token)->token);
				if (access(splitted_path[i], F_OK) == 0 && access(splitted_path[i], X_OK) == 0)
					break ;
				i++;
			}
			if (splitted_path[i] == NULL)
				return ;
			i = 0;
			while (splitted_path[i] != NULL)
			{
				free(splitted_path[i]);
				i++;
			}
			free(splitted_path);
		}
		(*token) = (*token)->next;
	}
	(*token) = temp;
}

/**
 * FUNCTION: (ignore)  finds already preset ignore trigger that is true
 * 				and sets the following token ignore triggers to true until
 * 				it finds one of the metacharacters. Ex. Pipe.
 */
static void	ignore(t_token **token)
{
	t_token *temp;

	temp = (*token);
	while ((*token) != NULL)
	{
		if ((*token)->ignore == true)
		{
			while ((*token) != NULL && ((*token)->name < PIPE || (*token)->name > R_AP_OUTPUT))
			{
				(*token)->ignore = true;
				(*token) = (*token)->next;
			}
		}
		if ((*token) != NULL)
			(*token) = (*token)->next;
	}
	(*token) = temp;
}

void register_tokens(t_info *info, t_token **token, t_env *env)
{
	t_token *temp_token;

	temp_token = (*token);
	expand_expansions(token, env);
	assign_indexes(token, info);
	connecting_quotes(token);
	//print_the_list("before syntax error", (*token));
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
