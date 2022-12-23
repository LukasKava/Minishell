/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 09:05:13 by pbiederm          #+#    #+#             */
/*   Updated: 2022/12/23 18:47:13 by lkavalia         ###   ########.fr       */
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
		empty_input_fd = open("/tmp/tmp_here.txt", \
		O_CREAT | O_RDONLY | O_TRUNC, 0644);
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

int	here_doc_read(char	*delimit)
{
	int		fd;
	char	*buff;

	fd = open("/tmp/here_dox.txt", O_CREAT | O_RDWR | O_TRUNC, 0666);
	if (fd == -1)
		perror(":");
	signal(SIGINT, handle_sigint);
	while (TRUE)
	{
		buff = readline("> ");
		if (ft_strncmp(buff, delimit, ft_strlen(delimit)) == 0 && \
		(ft_strlen(delimit) == ft_strlen(buff)))
			break ;
		write(fd, buff, strlen(buff));
		write(fd, "\n", 1);
		free(buff);
	}
	close(fd);
	fd = open("/tmp/here_dox.txt", O_CREAT | O_RDONLY, 0666);
	if (fd == -1)
		perror(":");
	free(buff);
	return (fd);
}

void	here_doc(char	*delimit, t_vars	*vars)
{
		vars->input_fd = here_doc_read(delimit);
}
