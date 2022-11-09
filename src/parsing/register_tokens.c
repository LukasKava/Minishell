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

static void message(t_token **token, int indentifier)
{
	(*token)->ignore = true;
	(*token)->indentifier = indentifier;
	printf("minishell: error unrecognised token: `%s'\n", (*token)->token);
}

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
static bool specials_outside(t_token **token)
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
}

static	int	check_for_spaces(char *str)
{
	int i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] != ' ')
		{
			printf("warum:");
			return (1);
		}
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
	if (specials_outside(token) == true)
		return;
	if ((*token)->token[0] == '|')
		(*token)->indentifier = PIPE;
	else if ((*token)->token[0] == '<' && (*token)->token[1] != '<')
		(*token)->indentifier = R_INPUT;
	else if ((*token)->token[0] == '<' && (*token)->token[1] == '<')
		(*token)->indentifier = R_AP_INPUT;
	else if ((*token)->token[0] == '>' && (*token)->token[1] != '>')
		(*token)->indentifier = R_OUTPUT;
	else if ((*token)->token[0] == '>' && (*token)->token[1] == '>')
		(*token)->indentifier = R_AP_OUTPUT;
	else if ((*token)->token[0] == '\\')
		(*token)->indentifier = ESCAPE;
	else if ((*token)->token[0] == '\0')
		(*token)->indentifier = SPace;
	else if (check_for_spaces((*token)->token) == 0)
		(*token)->indentifier = SPace;
	else if ((*token)->token[0] == '-' && ft_isalpha((*token)->token[1]) == 1)
		(*token)->indentifier = FLAG;
	else
		(*token)->indentifier = ELSE;
}

/**
 * FUNCTION: (recognise_builtins)	checks for the availible builtins and marks
 * 				the token if it is a possible builtin. Possibilities are:
 * 		echo		*	unset
 * 		cd			*	env
 * 		pwd			*	exit
 * 		export
 */
static void recognise_builtins(t_token **token)
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
		(*token)->indentifier = BUILT_IN;
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
			info->exit_status = 2;
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
 * 				heredoc the next token would get assingned the corect indentifier:
 * 		redirect input ('<') indentifier 2 -> next_indentifier = 11 input file
 * 		redirect output ('>') indentifier 3 -> next_indentifier = 12 output file
 *		Here document ('<<') indentifier 4 -> next_indentifier = 13 delimitor
 *		Redirect output append ('>>') inden.. 5 -> next_indentifier = 12 output file
 */
static void register_next_token(t_token **token)
{
	if ((*token)->indentifier == R_INPUT)		  //	Redirect input
		(*token)->next->indentifier = INPUT_F; // Input file
	else if ((*token)->indentifier == R_OUTPUT)  //	Redirect output
		(*token)->next->indentifier = OUTPUT_F; // output_file
	else if ((*token)->indentifier == R_AP_INPUT)  //	Redirect input append || here-documents
		(*token)->next->indentifier = DELIMITOR; // delimitor
	else if ((*token)->indentifier == R_AP_OUTPUT)  //	Redirect output append
		(*token)->next->indentifier = OUTPUT_F; // output_file
}

static void recognise_commands(t_token **token)
{
	t_token *temp;

	temp = (*token);
	while ((*token) != NULL)
	{
		if ((*token)->indentifier == BUILT_IN || (*token)->indentifier == ELSE)
		{
			if ((*token)->indentifier == ELSE) // Beggining != builtin = command
				(*token)->indentifier = COMMAND;	 // Command
			(*token) = (*token)->next;
			while ((*token) != NULL && ((*token)->indentifier == ELSE || \
			(*token)->indentifier == SPace || (*token)->indentifier > ESCAPE || (*token)->ignore == true))
			{
				if ((*token)->indentifier == ELSE)
				{
					printf("token token: %s\n", (*token)->token);
					(*token)->indentifier = ARGUMENT;
				}
				(*token) = (*token)->next;
			}
			if ((*token) == NULL)
				break;
		}
		if ((*token)->indentifier >= PIPE && (*token)->indentifier <= R_AP_OUTPUT)
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

static void check_command_excists(t_token **token, char **envp)
{
	int i;
	char *path;
	char **splitted_path;
	t_token *temp;

	i = 0;
	path = NULL;
	splitted_path = NULL;
	while (envp[i] != ft_strnstr(envp[i], "PATH", 5))
		i++;
	path = envp[i];
	printf("path: %s\n", path);
	temp = (*token);
	i = 0;
	while ((*token) != NULL)
	{
		if ((*token)->indentifier == COMMAND)
		{
			splitted_path = ft_split(path, ':');
			splitted_path[0] = ft_delete(splitted_path[0], "PATH=");
			while (splitted_path[i] != NULL)
			{
				splitted_path[i] = ft_strjoin(splitted_path[i], "/");
				splitted_path[i] = ft_strjoin(splitted_path[i], (*token)->token);
				if (access(splitted_path[i], F_OK) == 0 && access(splitted_path[i], X_OK) == 0)
				{
					printf("command: %s does excist: %s\n", (*token)->token, splitted_path[i]);
					break ;
				}
				i++;
			}
			if (splitted_path[i] == NULL)
			{
				printf("command: %s does not excist!\n", (*token)->token);
				(*token)->ignore = true;
			}
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
			while ((*token) != NULL && ((*token)->indentifier < PIPE || (*token)->indentifier > R_AP_OUTPUT))
			{
				printf("token->token: %s\n", (*token)->token);
				(*token)->ignore = true;
				(*token) = (*token)->next;
			}
		}
		if ((*token) != NULL)
			(*token) = (*token)->next;
	}
	(*token) = temp;
}

void register_tokens(t_info *info, t_token **token, char **envp)
{
	t_token *temp_token;

	temp_token = (*token);
	assign_indexes(token, info);
	if (info->error == false)
		check_tokens(info, token); // WORKS NEEDS REVIEW
	print_the_list("inside", (*token));
	if (info->error == false)
	{
		recognise_commands(token);
		check_command_excists(token, envp);
//		printf("after\n");
		ignore(token);
//		printf("went through ignore\n");
	}
	printf("after recognise commands\n");
	(*token) = temp_token;
}