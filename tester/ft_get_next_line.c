/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_next_line.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkaufman <rkaufman@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/01 13:44:10 by rkaufman          #+#    #+#             */
/*   Updated: 2022/04/07 06:34:55 by rkaufman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_tester.h"

static char	*ft_gnl_handling(char **buffer, int fd, char *output);
static void	ft_free_buffer(char **buffer, int fd);
static char	*ft_found_new_line(char *buffer, char *output);

char	*ft_get_next_line(int fd)
{
	static char	*buffer[MAX_FD];
	char		*output;

	if (fd < 0 || fd > MAX_FD)
		return (NULL);
	output = (char *) malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!output)
		return (NULL); 
	output[0] = '\0';
	if (!buffer[fd])
	{
		buffer[fd] = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
		buffer[fd][0] = '\0';
	}
	return (ft_gnl_handling(buffer, fd, output));
}

static char	*ft_gnl_handling(char **buffer, int fd, char *output)
{
	ssize_t	read_result;

	while (1)
	{
		if (buffer[fd][0])
		{
			if (ft_char_in_str(buffer[fd], '\n'))
				return (ft_found_new_line(buffer[fd], output));
			output = ft_realloc(output, buffer[fd], 1, 0);
		}
		read_result = read(fd, (void *) buffer[fd], BUFFER_SIZE);
		if (read_result == -1 || (read_result == 0 && output[0] == '\0'))
		{
			free(output);
			ft_free_buffer(buffer, fd);
			return (NULL);
		}
		buffer[fd][read_result] = '\0';
		if (read_result == 0)
		{
			ft_free_buffer(buffer, fd);
			return (output);
		}
	}
}

static void	ft_free_buffer(char **buffer, int fd)
{
	free(buffer[fd]);
	buffer[fd] = NULL;
}

static char	*ft_found_new_line(char *buffer, char *output)
{
	size_t	copy_size;
	char	*tmp;

	copy_size = (ft_char_in_str(buffer, '\n') - buffer) + 2;
	tmp = (char *) malloc(sizeof(char) * copy_size);
	if (!tmp)
		return (NULL);
	tmp[0] = '\0';
	ft_copy(tmp, buffer, copy_size);
	output = ft_realloc(output, tmp, 1, 1);
	buffer[0] = '\0';
	ft_copy(buffer, &buffer[copy_size - 1], 0);
	return (output);
}
