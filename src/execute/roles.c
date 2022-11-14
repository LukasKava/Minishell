/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   roles.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbiederm <pbiederm@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 09:22:21 by pbiederm          #+#    #+#             */
/*   Updated: 2022/11/14 09:22:28 by pbiederm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	roles_expanded(int **fd, t_chunk	*salt, t_info *info, char	**envp)
{
	
	if (salt->indentifier == CMD_BLOCK && salt->next == NULL)
		run(salt, info, envp);
	else if (salt->indentifier == CMD_BLOCK && salt->next->indentifier == DELIMITOR && salt->next->next == NULL)
		here_doc(&salt, info, envp);
	else if (salt->indentifier == CMD_BLOCK && salt->next->indentifier == INPUT_F && salt->next->next == NULL)
		input_first(fd, salt, info, envp);
	else if (salt->indentifier == CMD_BLOCK && (salt->indentifier == R_AP_OUTPUT_F||salt->next->indentifier == OUTPUT_F) && salt->next->next == NULL)
		output_first(fd, salt, info, envp);
	else
		input_output(fd, salt, info, envp);
}