/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbiederm <pbiederm@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 09:05:13 by pbiederm          #+#    #+#             */
/*   Updated: 2022/12/22 15:03:08 by pbiederm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	here_doc_handle(t_chunk **salt, t_vars *vars)
{
	t_chunk	*element;
	int		empty_input_fd;

	element = *salt;
	empty_input_fd = -1;
	if (pipe_this_node(&element))
		here_doc(element->in_f[vars->number_of_infiles].name, vars);
	else if (element->next == NULL)
		here_doc(element->in_f[vars->number_of_infiles].name, vars);
	else
	{
		empty_input_fd = open("./includes/tmp_in.txt", \
		O_CREAT | O_RDWR | O_TRUNC, 0644);
		if (empty_input_fd == -1)
		{
			write(2, "Error while opening temporary input file\n", 42);
			g_errors.g_exit_status = 1;
		}
		dup2(empty_input_fd, STDIN_FILENO);
		close(empty_input_fd);
	}
}

void	pipe_error(void)
{
	g_errors.g_exit_status = 1;
	write(2, "Problems with setting up the here doc pipe\n", 44);
	perror(" ");
}

void	here_doc_read(int pfd_output, char	*delimit)
{
	char	*buff;

	signal(SIGINT, handle_sigint);
	while (TRUE && g_errors.bip == false)
	{
		buff = readline("> ");
		if (!buff)
		{
			close(pfd_output);
			write(2, "\033[0;31mCtrl-D was activated\033[0m\n", 33);
			exit(1);
		}
		if (ft_strncmp(buff, delimit, ft_strlen(delimit)) == 0 && \
		(ft_strlen(delimit) == ft_strlen(buff)))
			break ;
		write(pfd_output, buff, strlen(buff));
		write(pfd_output, "\n", 1);
	}
}

void	here_doc(char	*delimit, t_vars	*vars)
{
	int		pfd[2];
	int		pid;

	if (pipe(pfd) == -1)
		pipe_error();
	vars->input_fd = dup(pfd[INPUT]);
	close(pfd[INPUT]);
	pid = fork();
	if (pid < 0)
		write(2, "Here doc error\n", 16);
	if (pid == 0)
	{
		here_doc_read(pfd[OUTPUT], delimit);
		close(pfd[OUTPUT]);
		exit(0);
	}
	else
	{
		wait(NULL);
		close(pfd[OUTPUT]);
	}
}
