/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkaufman <rkaufman@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/25 16:17:56 by rkaufman          #+#    #+#             */
/*   Updated: 2022/04/07 06:36:17 by rkaufman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_tester.h"

void	ft_write_fd(int fd, char *s)
{
	write(fd, s, ft_strlen(s));
}

void	ft_write_fd_nl(int fd, char *s)
{
	write(fd, s, ft_strlen(s));
	write(fd, "\n", 1);
}

int	ft_write_err(char *output)
{
	ft_write_fd(2, COLOR_RED);
	ft_write_fd(2, output);
	ft_write_fd(2, COLOR_DEFAULT);
	return (-1);
}

void	ft_write_test_cmd_to(char *cmd, char *to)
{
	char	*tmp;

	tmp = ft_replace_new_line(cmd);
	ft_write_fd(1, "send to ");
	ft_write_fd(1, to);
	ft_write_fd(1, " =>["COLOR_YELLOW);
	ft_write_fd(1, tmp);
	ft_write_fd(1, COLOR_DEFAULT"]\n");
	free(tmp);
}

char	*ft_check_path(char *cmd, char **paths)
{
	char	*test_path;
	int		i;

	i = 0;
	while (paths[i])
	{
		test_path = ft_realloc(paths[i], "/", 0, 0);
		test_path = ft_realloc(test_path, cmd, 1, 0);
		if (access(test_path, F_OK) == 0)
			return (test_path);
		free(test_path);
		i++;
	}
	return (NULL);
}

int	ft_str_var_cmp(char *var_name, char *var_elem)
{
	int	i;

	i = 0;
	while (var_name[i])
	{
		if (var_name[i] != var_elem[i])
			return (0);
		i++;
	}
	if (var_elem[i] == '=' || var_elem[i] == '\0')
		return (1);
	return (0);
}

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s && s[i])
		i++;
	return (i);
}

void	*ft_memset(void *s, int c, size_t n)
{
	unsigned char		*ptr;
	unsigned char		value;
	size_t				i;

	if (n > 0)
	{
		i = 0;
		ptr = (unsigned char *) s;
		value = (unsigned char) c;
		while (i < n)
		{
			*ptr = value;
			ptr++;
			i++;
		}
	}
	return (s);
}

size_t	ft_copy(char *dst, char *src, size_t size)
{
	size_t	i;

	i = 0;
	if (src && size == 0)
	{
		while (src[i])
		{
			dst[i] = src[i];
			i++;
		}
	}
	else if (src)
	{
		size--;
		while (i < size && src[i])
		{
			dst[i] = src[i];
			i++;
		}
	}
	dst[i] = '\0';
	return (i);
}

char	*ft_string_dup(const char *s)
{
	char	*output;

	output = (char *) malloc(ft_strlen(s) + 1);
	if (!output)
		return (NULL);
	ft_copy(output, (char *)s, 0);
	return (output);
}

char	*ft_get_substring(char const *s, size_t start, size_t len)
{
	char	*substring;

	if (!s)
		return (NULL);
	substring = (char *) malloc(len + 1);
	if (!substring)
		return (NULL);
	ft_copy(substring, (char *) &s[start], len + 1);
	return (substring);
}

char	*ft_int_to_string(long n)
{
	char	*output;
	long	tmp;
	size_t	i;
	int		sign;

	sign = 1;
	if (n < 0)
		sign = -1;
	tmp = (long) n * sign;
	i = ft_get_int_len(tmp, sign);
	output = malloc(i + 1);
	if (!output)
		return (NULL);
	output[i--] = '\0';
	while (tmp > 9)
	{
		output[i] = (char)(tmp % 10) + '0';
		tmp = tmp / 10;
		i--;
	}
	output[i] = (char)(tmp % 10) + '0';
	if (sign == -1)
		output[0] = '-';
	return (output);
}

size_t	ft_get_int_len(long n, int sign)
{
	size_t	i;

	i = 1;
	while (n > 9)
	{
		n /= 10;
		i++;
	}
	if (sign == -1)
		i++;
	return (i);
}

size_t	ft_count_of_in_str(const char *s, const char c)
{
	size_t	count;

	count = 0;
	while (*s)
	{
		if (*s == c)
			count++;
		s++;
	}
	return (count);
}

char	*ft_char_in_str(const char *s, const char c)
{
	while (*s)
	{
		if (*s == c)
			return ((char *)s);
		s++;
	}
	return (NULL);
}

char	*ft_realloc(char *s1, char *s2, int free_s1, int free_s2)
{
	ssize_t		s1_len;
	char		*output;

	s1_len = ft_strlen(s1);
	output = (char *)malloc(s1_len + ft_strlen(s2) + 1);
	if (!output)
		return (NULL);
	ft_copy(output, s1, 0);
	ft_copy(&output[s1_len], s2, 0);
	if (free_s1)
		free(s1);
	if (free_s2)
		free(s2);
	return (output);
}

int		ft_pos_in_string(char *s, char c)
{
	int	i;

	i = 0;
	while (s[i] && s[i] != c)
		i++;
	return (i);
}

char	*ft_remove_char(char *s, char c)
{
	char	*output;
	char	*tmp;
	int		i;
	int		count;

	count = ft_count_of_in_str(s, c);
	if (count == 0)
		return (s);
	tmp = s;
	output = (char *) malloc((ft_strlen(s) - count) + 1);
	if (!output)
		return (NULL);
	i = 0;
	while (*s)
	{
		if (*s != c)
		{
			output[i] = *s;
			i++;
		}
		s++;
	}
	output[i] = '\0';
	free(tmp);
	return (output);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	if (!s1 && !s2)
		return (1);
	if (!s1 || !s2)
		return (0);
	while (s1[i] && s2[i])
	{
		if (s1[i] != s2[i])
			return (0);
		i++;
	}
	if (s1[i] == '\0' && s2[i] == '\0')
		return (1);
	return (0);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		if (s1[i] != s2[i])
			return (s1[i] - s2[i]);
		else if (s1[i] == '\0' || s2[i] == '\0')
			return (0);
		i++;
	}
	return (0);
}

char	*ft_replace_new_line(char *input)
{
	char	*output;
	int		count;
	int		i_input;
	int		i_output;

	if (!input)
		return (NULL);
	count = ft_count_of_in_str(input, '\n');
	output = (char *) malloc((count * 2) + ft_strlen(input) + 1);
	if (!output)
		return (NULL);
	i_input = 0;
	i_output = 0;
	while (input[i_input])
	{
		if (input[i_input] != '\n')
			output[i_output] = input[i_input];
		else
		{
			output[i_output] = '\\';
			i_output++;
			output[i_output] = 'n';
		}
		i_input++;
		i_output++;
	}
	output[i_output] = '\0';
	return (output);
}


char	*ft_getenv(char *var_name, char **envp)
{
	int	i;

	i = 0;
	while (envp[i] && !ft_str_var_cmp(var_name, envp[i]))
		i++;
	if (envp[i])
		return (&envp[i][ft_pos_in_string(envp[i], '=') + 1]);
	else
		return (NULL);
}

/*
char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	little_len;
	char	*ptr;

	i = 0;
	little_len = ft_strlen(little);
	ptr = (char *) big;
	if (little_len == 0)
		return (ptr);
	if (little_len > ft_strlen(big))
		return (NULL);
	while (i + little_len <= len)
	{
		if (big[i] == little[0])
		{
			if (ft_strncmp(&big[i], little, little_len) == 0)
				return (&ptr[i]);
		}
		i++;
	}
	return (NULL);
}
*/
