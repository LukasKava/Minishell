/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbiederm <pbiederm@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/19 17:49:32 by pbiederm          #+#    #+#             */
/*   Updated: 2022/11/10 11:09:27 by pbiederm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/get_next_line.h"

void	*ft_calloc(size_t count, size_t size)
{
	char	*point_to_calloc;
	size_t	t;

	if (count == SIZE_MAX || size == SIZE_MAX)
		return (NULL);
	t = count * size;
	point_to_calloc = malloc(t);
	if (!point_to_calloc)
		return (NULL);
	ft_bzero(point_to_calloc, t);
	return (point_to_calloc);
}

char	*static_ret(char *arr)
{
	char	*tmp;
	char	*ret;
	size_t	ndl;

	ndl = 0;
	tmp = ft_substr(arr, 0, ft_strlen(arr));
	while (tmp[ndl] != '\n')
		ndl++;
	ret = ft_substr(tmp, 0, ndl + 1);
	ft_bzero(arr, ft_strlen(arr));
	ft_strlcat(arr, &tmp[ndl + 1], ft_strlen(tmp));
	free (tmp);
	return (ret);
}

char	*buff_ret(char *arr, char *bufree)
{
	char	*tmp;
	char	*ret;
	size_t	ndl;

	free (bufree);
	ndl = 0;
	tmp = ft_substr(arr, 0, ft_strlen(arr));
	while (tmp[ndl] != '\n')
		ndl++;
	ft_bzero(arr, ft_strlen(arr));
	ft_strlcat(arr, &tmp[ndl + 1], ft_strlen(tmp));
	ret = ft_substr(tmp, 0, ndl + 1);
	free(tmp);
	return (ret);
}

char	*end_ret(char *arr, char *bufree)
{
	char	*ret;

	free(bufree);
	ret = ft_substr(arr, 0, ft_strlen(arr));
	ft_bzero(arr, ft_strlen(arr));
	return (ret);
}

char	*get_next_line(int fd)
{
	static char		storage[800];
	char			*buffy;
	int				bytes;

	if (fd < 0)
		return (NULL);
	if (ft_strchr(storage, '\n'))
		return (static_ret(storage));
	buffy = (char *)ft_calloc(BUFFER_SIZE + 1, sizeof(char));
	while (!ft_strchr(storage, '\n'))
	{
		bytes = read(fd, buffy, BUFFER_SIZE);
		if (bytes <= 0 && storage[0] == 0)
			break ;
		ft_strlcat(storage, buffy, ft_strlen(storage) + bytes + 1);
		if (ft_strchr(storage, '\n'))
			return (buff_ret(storage, buffy));
		if (bytes < BUFFER_SIZE)
			return (end_ret(storage, buffy));
	}
	free(buffy);
	return (NULL);
}

// int	main(void)
// {
// 	int	fd_to_read = open("text.txt", O_RDONLY);
// 	int	i;

// 	i = 0;
// 	while (i < 4)
// 	{
// 		printf("gnl: %s", get_next_line(fd_to_read));
// 		i++;
// 	}
// 	close (fd_to_read);
// }
