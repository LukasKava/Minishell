/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   register_tokens2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/18 02:10:49 by lkavalia          #+#    #+#             */
/*   Updated: 2022/12/18 12:55:04 by lkavalia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	register_redirection_name(t_token **token, int redirection)
{
	if ((*token)->next->name == SPC)
		(*token)->next->next->name = redirection;
	else
		(*token)->next->name = redirection;
}

/**
 * FUNCTION: (register_next_token) makes sure that in case of redirections or
 * 				heredoc the next token would get assingned the corect name:
 * 		redirect input ('<') name 2 -> next_name = 11 input file
 * 		redirect output ('>') name 3 -> next_name = 12 output file
 *		Here document ('<<') name 4 -> next_name = 13 delimitor
 *		Redirect output append ('>>') inden.. 5 -> next_name = 12 output file
 */
void	register_next_token(t_token **token)
{
	if ((*token)->name == R_INPUT)
		register_redirection_name(token, INPUT_F);
	else if ((*token)->name == R_OUTPUT)
		register_redirection_name(token, OUTPUT_F);
	else if ((*token)->name == R_AP_INPUT)
		register_redirection_name(token, DELIMITOR);
	else if ((*token)->name == R_AP_OUTPUT)
		register_redirection_name(token, R_AP_OUTPUT_F);
}

/**
 * FUNCTION: (recognise_builtins)	checks for the availible builtins and marks
 * 				the token if it is a possible builtin. Possibilities are:
 * 		echo		*	unset
 * 		cd			*	env
 * 		pwd			*	exit
 * 		export
 */
void	recognise_builtins(t_token **token)
{
	int	check;

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

static void	else_to_arg(t_token **token)
{
	while ((*token) != NULL && ((*token)->name == ELSE || \
								(*token)->name == SPC || \
								(*token)->name == EMPTY || \
								(*token)->name > ESCAPE || \
								(*token)->ignore == true))
	{
		if ((*token)->name == ELSE)
			(*token)->name = ARGUMENT;
		(*token) = (*token)->next;
	}
}

void	recognise_commands(t_token **token)
{
	t_token	*temp;

	temp = (*token);
	while ((*token) != NULL)
	{
		if ((*token)->name == BUILT_IN || (*token)->name == ELSE)
		{
			if ((*token)->name == ELSE)
				(*token)->name = COMMAND;
			(*token) = (*token)->next;
			else_to_arg(token);
			if ((*token) == NULL)
				break ;
		}
		if ((*token)->name >= PIPE && (*token)->name <= R_AP_OUTPUT)
			register_next_token(token);
		(*token) = (*token)->next;
	}
	(*token) = temp;
}
