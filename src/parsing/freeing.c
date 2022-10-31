/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   freeing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 12:37:13 by lkavalia          #+#    #+#             */
/*   Updated: 2022/10/31 13:59:08 by lkavalia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void freeing_tokens(t_token *token)
{
	t_token *free_token;

	free_token = token;
	while (token != NULL)
	{

		free_token = token;
		printf("[%s] ---> ", free_token->token);
		free(token->token);
		token = token->next;
		//	free(free_token->token);
		free(free_token);
		printf("token freed succesfully!\n");
	}
	free(token);
}