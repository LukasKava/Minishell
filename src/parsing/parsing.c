/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 13:05:50 by lkavalia          #+#    #+#             */
/*   Updated: 2022/12/19 13:51:11 by lkavalia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * FUNCTION: (find_arguments) takes care of finding and registering command and
 * 				its arguments. The function can be devided into 3 parts:
 * 	1. Checking how many spots we will need for our 2d array.
 * 	2. Giving a chunck->arguments that space.
 * 	3. filling up the available spots with command and arguments.
 */
t_token	*find_arguments(t_token *token, t_chunk **chunk)
{
	int		i;
	int		length;
	t_token	*temp;

	i = 0;
	length = 0;
	temp = token;
	while (token != NULL && token->name != PIPE)
	{
		if (lenght_picker(token) == 0)
			length++;
		token = token->next;
	}
	(*chunk)->arguments = ft_calloc(length + 1, sizeof(char *));
	(*chunk)->arguments[length] = NULL;
	token = temp;
	while (token != NULL && token->name != PIPE)
	{
		i = filling_arguments(token, chunk, i);
		token = token->next;
	}
	return (token);
}

static void	save_the_files(int amount, int id, t_token *token, t_redir **red)
{
	int	i;

	i = 0;
	(*red) = ft_calloc(amount + 1, sizeof(t_redir));
	(*red)[amount].name = NULL;
	while (token != NULL && token->name != PIPE && amount >= 0)
	{
		if ((token->name == id || token->name == id + 1) && \
								(token->next != NULL))
		{
			token = token->next;
			while (token != NULL && token->name == SPC)
				token = token->next;
			if (token != NULL && (token->name == id + 9 || \
									token->name == id + 10))
			{
				(*red)[i].name = token->t;
				(*red)[i].type = token->name;
				i++;
				amount--;
			}
		}
		if (token != NULL && token->name != PIPE)
			token = token->next;
	}
}

static void	count_inputs(t_token *token, t_chunk **chunk, int id)
{
	t_token	*temp;
	int		count;
	int		ap_count;

	temp = token;
	count = 0;
	ap_count = 0;
	if (id == R_AP_INPUT || id == R_AP_OUTPUT)
		id--;
	while (token != NULL && token->name != PIPE)
	{
		if (token->name == id)
			count++;
		if (token->name == id + 1)
			ap_count++;
		token = token->next;
	}
	token = temp;
	if ((count + ap_count) != 0 && id == R_INPUT)
		save_the_files(count + ap_count, id, token, &(*chunk)->in_f);
	else if ((count + ap_count) != 0 && id == R_OUTPUT)
		save_the_files(count + ap_count, id, token, &(*chunk)->out_f);
}

static void	register_the_redirections(t_token *token, t_chunk **chunk)
{
	int	registered_input;
	int	registered_output;

	registered_input = 0;
	registered_output = 0;
	while (token != NULL && token->name != PIPE)
	{
		if (registered_input != 1 && (token->name == R_INPUT || \
										token->name == R_AP_INPUT) && (token))
		{
			registered_input = 1;
			count_inputs(token, chunk, token->name);
		}
		else if (registered_output != 1 && (token->name == R_OUTPUT || \
										token->name == R_AP_OUTPUT))
		{
			registered_output = 1;
			count_inputs(token, chunk, token->name);
		}
		token = token->next;
	}
}

void	get_the_commands(t_token *t, t_env *env, t_chunk **c, t_info *i)
{
	t_chunk	*temp;

	temp = (*c);
	while (t != NULL)
	{
		register_the_redirections(t, c);
		while (t != NULL && t->name != PIPE)
		{
			t = choosing_block(t, c, env);
			if (t != NULL && t->name != PIPE)
				t = t->next;
		}
		if (t != NULL && t->index != 0 && t->name == PIPE)
			(*c) = attach_chunk_end(*c, i);
		if (t != NULL)
			t = t->next;
	}
	(*c)->next = NULL;
	(*c) = temp;
}
