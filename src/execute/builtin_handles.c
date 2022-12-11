/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_handles.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbiederm <pbiederm@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/11 11:24:38 by pbiederm          #+#    #+#             */
/*   Updated: 2022/12/11 11:27:21 by pbiederm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"../../includes/minishell.h"

void	echo_handle(t_chunk	**salt)
{
	t_chunk *element;

	element = *salt;
	if(element->indentifier == BUILT_IN_BLOCK)
	{
		if (strncmp(element->arguments[0],"echo", strlen("echo")) == 0)
		{
			builtins_echo(element->fd[1], element->arguments);
		}
	}	
}

void	cd_handle(t_chunk	**salt, t_env	*env)
{
	t_chunk *element;

	element = *salt;
	if(element->indentifier == BUILT_IN_BLOCK)
	{
		if (strncmp(element->arguments[0],"cd", strlen("cd")) == 0)
		{
			builtins_cd(element->arguments, &env);
		}
	}	
}

void	pwd_handle(t_chunk	**salt)
{
	t_chunk *element;

	element = *salt;
	if(element->indentifier == BUILT_IN_BLOCK)
	{
		if (strncmp(element->arguments[0],"pwd", strlen("pwd")) == 0)
		{
			builtins_pwd(element->fd[1]);
		}
	}	
}