/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_threads.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkaufman <rkaufman@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/02 17:36:49 by rkaufman          #+#    #+#             */
/*   Updated: 2022/04/03 10:47:57 by rkaufman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_tester.h"

int	ft_create_threads(t_data *data)
{
	pthread_mutex_init(&data->mini.lock, NULL);
	pthread_mutex_init(&data->bash.lock, NULL);
	pthread_create(&data->mini.thread_read, NULL, &ft_thread_read_mini, (void *) data);
	pthread_create(&data->bash.thread_read, NULL, &ft_thread_read_bash, (void *) data);
	return (1);
}

int	ft_destroy_threads(t_data *data)
{
	pthread_join(data->mini.thread_read, NULL);
	pthread_join(data->bash.thread_read, NULL);
	pthread_mutex_destroy(&data->mini.lock);
	pthread_mutex_destroy(&data->bash.lock);
	return (1);
}

void	*ft_thread_read_mini(void *input)
{
	t_data	*data;
	char	*tmp;

	data =  (t_data *) input;
	tmp = NULL;
	while (1)
	{
		tmp = ft_read_from_minishell(data);
		if (!tmp)
			return (NULL);
		pthread_mutex_lock(&data->mini.lock);
		ft_lstadd_back(&data->mini.read_result, ft_lstnew(tmp));
		free(tmp);
		tmp = NULL;
		pthread_mutex_unlock(&data->mini.lock);
	}
	return (NULL);
}

void	*ft_thread_read_bash(void *input)
{
	t_data	*data;
	char	*tmp;

	data =  (t_data *) input;
	tmp = NULL;
	while (1)
	{
		tmp = ft_read_from_bash(data);
		if (!tmp)
			return (NULL);
		pthread_mutex_lock(&data->bash.lock);
		ft_lstadd_back(&data->bash.read_result, ft_lstnew(tmp));
		free(tmp);
		tmp = NULL;
		pthread_mutex_unlock(&data->bash.lock);

	}
	return (NULL);
}
