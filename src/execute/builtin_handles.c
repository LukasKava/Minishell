/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_handles.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/11 11:24:38 by pbiederm          #+#    #+#             */
/*   Updated: 2022/12/16 12:21:57 by lkavalia         ###   ########.fr       */
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
			builtins_echo(STDOUT_FILENO, element->arguments);
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
			builtins_pwd(STDOUT_FILENO);
		}
	}
}

void	env_handle(t_chunk **salt, t_env *e_list)
{
	t_chunk *element;

	element = *salt;
	if(element->indentifier == BUILT_IN_BLOCK)
	{
		if (strncmp(element->arguments[0],"env", strlen("env")) == 0)
		{
			builtins_env(element->arguments, e_list);
		}
	}	
	
}

void	export_handle(t_env **exp_list, t_env **e_l, t_chunk **salt, int fd)
{
	t_chunk *element;

	element = *salt;
	if(element->indentifier == BUILT_IN_BLOCK)
	{
		if (strncmp(element->arguments[0],"export", strlen("export")) == 0)
		{
			builtins_export(exp_list, e_l, element->arguments, fd);
		}
	}
}

void	unset_handle(t_env **exp_l, t_env **env_l, char **env, t_chunk	**salt)
{
	t_chunk	*element;
	
	element = *salt;
	if(element->indentifier == BUILT_IN_BLOCK)
	{
		if (strncmp(element->arguments[0],"unset", strlen("unset")) == 0)
		{
			builtins_unset(exp_l, env_l, env, element->arguments);
		}
	}
}

void	exit_handle(t_data *hive, t_chunk **salt)
{
	t_chunk	*element;
	
	element = *salt;
	if(element->indentifier == BUILT_IN_BLOCK)
	{
		if (strncmp(element->arguments[0],"exit", strlen("exit")) == 0)
		{
			g_exit_status = builtins_exit(hive, element->arguments);
		}
	}
}