/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   regiter_tokens.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 12:37:24 by lkavalia          #+#    #+#             */
/*   Updated: 2022/10/31 12:44:30 by lkavalia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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
		message(token, -7);
	else if ((*token)->token[0] == '&' && (*token)->token[1] != '&')
		message(token, -6);
	else if ((*token)->token[0] == '&' && (*token)->token[1] == '&')
		message(token, -5);
	else if ((*token)->token[0] == ';' && (*token)->token[1] != ';')
		message(token, -4);
	else if ((*token)->token[0] == ';' && (*token)->token[1] == ';')
		message(token, -3);
	else if ((*token)->token[0] == '(')
		message(token, -2);
	else if ((*token)->token[0] == ')')
		message(token, -1);
	else if ((*token)->token[0] == '\\')
		message(token, 6);
	if ((*token)->ignore == true)
		return (true);
	return (false);
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
		(*token)->indentifier = 1;
	else if ((*token)->token[0] == '<' && (*token)->token[1] != '<')
		(*token)->indentifier = 2;
	else if ((*token)->token[0] == '<' && (*token)->token[1] == '<')
		(*token)->indentifier = 4;
	else if ((*token)->token[0] == '>' && (*token)->token[1] != '>')
		(*token)->indentifier = 3;
	else if ((*token)->token[0] == '>' && (*token)->token[1] == '>')
		(*token)->indentifier = 5;
	else if ((*token)->token[0] == '\\')
		(*token)->indentifier = 6;
	else if ((*token)->token[0] == '\0')
		(*token)->indentifier = 0;
	else if ((*token)->token[0] == '-' && ft_isalpha((*token)->token[1]) == 1)
		(*token)->indentifier = 10;
	else
		(*token)->indentifier = -9;
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
		check = 8;
	else if (ft_strcmp((*token)->token, "cd") == 0)
		check = 8;
	else if (ft_strcmp((*token)->token, "pwd") == 0)
		check = 8;
	else if (ft_strcmp((*token)->token, "export") == 0)
		check = 8;
	else if (ft_strcmp((*token)->token, "unset") == 0)
		check = 8;
	else if (ft_strcmp((*token)->token, "env") == 0)
		check = 8;
	else if (ft_strcmp((*token)->token, "exit") == 0)
		check = 8;
	if (check == 8)
		(*token)->indentifier = 8;
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
	if ((*token)->indentifier == 2)		  //	Redirect input
		(*token)->next->indentifier = 11; // Input file
	else if ((*token)->indentifier == 3)  //	Redirect output
		(*token)->next->indentifier = 12; // output_file
	else if ((*token)->indentifier == 4)  //	Redirect input append || here-documents
		(*token)->next->indentifier = 13; // delimitor
	else if ((*token)->indentifier == 5)  //	Redirect output append
		(*token)->next->indentifier = 12; // output_file
}

static void recognise_commands(t_token **token)
{
	t_token *temp;

	temp = (*token);
	while ((*token) != NULL)
	{
		if ((*token)->indentifier == 8 || (*token)->indentifier == -9)
		{
			if ((*token)->indentifier == -9) // Beggining != builtin = command
				(*token)->indentifier = 7;	 // Command
			(*token) = (*token)->next;
			while ((*token) != NULL && ((*token)->indentifier == -9 ||
										(*token)->indentifier > 6 || (*token)->ignore == true))
			{
				if ((*token)->indentifier == -9)
					(*token)->indentifier = 9;
				(*token) = (*token)->next;
			}
			if ((*token) == NULL)
				break;
		}
		if ((*token)->indentifier >= 0 && (*token)->indentifier <= 5)
			register_next_token(token);
		(*token) = (*token)->next;
	}
	(*token) = temp;
}

static void check_command_excists(t_token **token, char **envp)
{
	int i;
	char *path;
	char **splitted_path;
	t_token *temp;

	i = 0;
	while (envp[i] != ft_strnstr(envp[i], "PATH", 5))
		i++;
	path = envp[i];
	//	splitted_path = ft_split(path, ':');
	temp = (*token);
	//	splitted_path[0] = ft_strtrim(splitted_path[0], "PATH=");
	i = 0;
	while ((*token) != NULL)
	{
		if ((*token)->indentifier == 7)
		{
			printf("path: %s\n", path);
			splitted_path = ft_split(path, ':');
			splitted_path[0] = ft_strtrim(splitted_path[0], "PATH=");
			while (splitted_path[i] != NULL)
			{
				splitted_path[i] = ft_strjoin(splitted_path[i], "/");
				splitted_path[i] = ft_strjoin(splitted_path[i], (*token)->token);
				if (access(splitted_path[i], F_OK) == 0)
				{
					printf("checking the path: %s\n", splitted_path[i]);
					if (access(splitted_path[i], X_OK) == 0)
					{
						printf("Command excists and can be executed!\n");
						break;
					}
				}
				i++;
			}
			if (splitted_path[i] == NULL)
			{
				printf("command: %s does not excist!\n", (*token)->token);
				(*token)->ignore = true;
			}
			//		i = 0;
			while (i >= 0)
			{
				free(splitted_path[i]);
				i--;
			}
			free(splitted_path);
		}
		i = 0;
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
	if (info->error == false)
	{
		recognise_commands(token);
		check_command_excists(token, envp);
	}
	printf("after recognise commands\n");
	(*token) = temp_token;
}
