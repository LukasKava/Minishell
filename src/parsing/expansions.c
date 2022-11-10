/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/10 15:32:15 by lkavalia          #+#    #+#             */
/*   Updated: 2022/11/10 17:50:40 by lkavalia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static size_t	ft_case(char c)
{
	if (c >= 'a' && c <= 'z')
		return (0);
	else if (c >= 'A' && c <= 'Z')
		return (1);
	return (2);
}

static int	find_expansion(char *str)
{
	int		i;
	bool	expansion_found;

	i = 0;
	expansion_found = false;
	while (str[i] != '\0')
	{
		if (str[i] == '$' && str[i + 1] != '\0')
		{
			i++;
			expansion_found = true;
			while (str[i] != '\0' && str[i] != ' ')
			{
				if (ft_case(str[i]) == 1 || str[i] == '_' || (str[i] >= '0' && str[i] <= '9'))
					i++;
				else	
					return (1);
			}
			if (expansion_found == true)
				return (0);
		}
		i++;
	}
	return (1);
}	

// static	void *get_rid_of_garbage(t_token **token)
// {
// 	char	*new_str;
// 	int		len;

// 	len = 0;
// 	new_str = 0;
// 	if ((*token)->double_quotes == true)
// 	{
// 		printf("More effort!\n");
// 	}
// 	else
// 	{
// 		printf("Less effort\n");
// 		while ((*token)->token[len] != '$')
// 			len++;
// 		new_str = ft_calloc(len, sizeof(char));
// 		new_str[len] = '\0';
// 		len = 0;
// 		while (new_str[len] != '\0')
// 		{
			 
// 			len++;
// 		}
// 	}
// 	free((*token)->token);
// 	return (new_str);
// }

//Legal rules of naming Variables in bash:
//	x	The variable name must be in the upper case as it is considered
//			good practise in bash scripting.
//	x	Insert the dolar sign "$" before the var name.
//	x	Do not use spaces after the initialiazation of the variable name
//			and its value.
//	x	A variable name can have letter/s.
//	X	A variable name can have numbers, underscores and digits.
void	expand_expansions(t_token **token, char **envp)
{
	t_token	*temp;

	temp = (*token);
	while ((*token) != NULL)
	{
		if ((*token)->single_quotes == false)
		{
			printf("IN for expansions!\n");
			if (find_expansion((*token)->token) == 0)
			{
				printf("EXPANSION: %s\n", (*token)->token);
			}
			// else
			// {
			// 	get_rid_of_garbage(*token);
			// }
		}
		(*token) = (*token)->next;
	}
	printf("envp: %s", envp[1]);
	(*token) = temp;
}