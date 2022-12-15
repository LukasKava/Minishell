/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_children.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkaufman <rkaufman@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/02 23:34:04 by rkaufman          #+#    #+#             */
/*   Updated: 2022/04/02 23:34:42 by rkaufman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_tester.h"

void	ft_minishell_child(t_data *data)
{
	char	**argv;

	argv = (char **) malloc(sizeof(char**) * 2);
	argv[0] = ft_string_dup("minishell");
	argv[1] = NULL;
	ft_close_main_pipes(data);
	dup2(data->mini_main.fd_read, STDIN_FILENO);
	close(data->mini_main.fd_read);
	dup2(data->mini_main.fd_write, STDOUT_FILENO);
	close(data->mini_main.fd_write);
	if (execve(data->mini_path, argv, data->envp) == -1)
		ft_write_fd(2, COLOR_RED"ERROR! Unable to execute minishell!\n"COLOR_DEFAULT);
	exit(0);
}

void	ft_bash_child(t_data *data)
{
	char	**argv;

	argv = (char **) malloc(sizeof(char**) * 2);
	argv[0] = ft_string_dup("bash");
	argv[1] = NULL;
	ft_close_main_pipes(data);
	dup2(data->bash_main.fd_read, STDIN_FILENO);
	close(data->bash_main.fd_read);
	dup2(data->bash_main.fd_write, STDOUT_FILENO);
	close(data->bash_main.fd_write);
	if (execve(data->bash_path, argv, data->envp) == -1)
		ft_write_fd(2, COLOR_RED"ERROR! Unable to execute bash!\n"COLOR_DEFAULT);
	exit(0);
}
