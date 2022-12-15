/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tester.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkaufman <rkaufman@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/01 21:56:12 by rkaufman          #+#    #+#             */
/*   Updated: 2022/04/07 10:23:36 by rkaufman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_tester.h"

int	main(int argc, char **argv, char **envp)
{
	t_data		data;

	if (argc != 2)
		return (ft_write_err("ERROR! Need one argument path to minishell!\n"));
	ft_alloc_mem_and_assign(&data, argv, envp);
	if (access(data.mini_path, F_OK) != 0)
		return (ft_write_err("ERROR! Could not find minishell executable!\n"));
	if (!ft_create_pipes(&data))
		return (ft_write_err("ERROR! Could not create pipes!\n"));
	data.fd_tests = open("tests", O_RDONLY);
	if (data.fd_tests == -1)
		return (ft_write_err("ERROR! Could not find minishell testfile!\n"));
	data.pid_mini = fork();
	if (data.pid_mini < 0)
		return (ft_write_err("ERROR! Could not fork!\n"));
	if (data.pid_mini == 0)
		ft_minishell_child(&data);
	data.bash_path = ft_get_bash_path(&data);
	if (!data.bash_path)
		return (ft_write_err("ERROR! Could not find bash executable!\n"));
	data.pid_bash = fork();
	if (data.pid_bash == 0)
		ft_bash_child(&data);
	if (data.pid_bash < 0)
		return (ft_write_err("ERROR! Could not fork!\n"));
	free(data.bash_path);
	free(data.mini_path);
	close(data.mini_main.fd_read);
	close(data.mini_main.fd_write);
	close(data.bash_main.fd_read);
	close(data.bash_main.fd_write);
	ft_create_threads(&data);
	ft_send_to_bash(&data, "cp tests bash_tests/\n");
	ft_send_to_bash(&data, "cp tests mini_tests/\n");
	ft_send_to_bash(&data, "cd bash_tests\n");
	usleep(10000);
	ft_send_to_minishell(&data, "cd mini_tests\n");
	usleep(10000);
	pthread_mutex_lock(&data.mini.lock);
	ft_delete_list(&data.mini.read_result);
	pthread_mutex_unlock(&data.mini.lock);
	ft_test_loop(&data);
	ft_write_summary(&data);
	//ft_write_fd(1, "out of while. wait for child to terminate.\n");
	ft_send_to_minishell(&data, "exit\n");
	waitpid(data.pid_mini, NULL, WUNTRACED);
	//ft_write_fd(1, "minishell terminated.\n");
	ft_send_to_bash(&data, "exit\n");
	waitpid(data.pid_bash, NULL, WUNTRACED);
	//ft_write_fd(1, "bash terminated.\n");
	//ft_write_fd(1, "wait for threads to terminate.\n");
	ft_destroy_threads(&data);
	ft_delete_list(&data.mini.read_result);
	ft_close_pipes(&data);
	return (0);
}

void	ft_write_summary(t_data *data)
{
	printf("\ncommands=[%i]\n"
			 "OK=[%i|%i]\n"
			 "KO=[%i]\n",
			 data->cmd_count, data->tests_passed, data->test_count, data->tests_failed);
	/*
	ft_write_fd(1, "\ncommands=[");
	ft_write_fd(1, ft_int_to_string(data->cmd_count));
	ft_write_fd(1, "]\n");
	ft_write_fd(1, "OK[");
	ft_write_fd(1, ft_int_to_string(data->tests_passed));
	ft_write_fd(1, "|");
	ft_write_fd(1, ft_int_to_string(data->test_count));
	ft_write_fd(1, "]\n");
	ft_write_fd(1, "KO=[");
	ft_write_fd(1, ft_int_to_string(data->tests_failed));
	ft_write_fd(1, "]\n");
	*/
}

void	ft_test_loop(t_data *data)
{
	char		*cmd;

	cmd = ft_get_next_cmd(data);
	while (cmd)
	{
		ft_write_test_cmd_to(cmd, "bash|mini");
		ft_send_to_bash(data, cmd);
		usleep(10000);
		ft_send_to_minishell(data, cmd);
		usleep(10000);
		pthread_mutex_lock(&data->mini.lock);
		pthread_mutex_lock(&data->bash.lock);
		if (data->mini.read_result)
		{
			//ft_printf_list(data->mini.read_result);
			ft_delete_elem(&data->mini.read_result, data->mini.read_result);//PROMPT
			ft_compare_result(data);
		}
		//ft_compare_result(data, data->bash.read_result, data->mini.read_result);
		ft_write_fd(1, "\n");
		ft_delete_list(&data->bash.read_result);
		ft_delete_list(&data->mini.read_result);
		pthread_mutex_unlock(&data->mini.lock);
		pthread_mutex_unlock(&data->bash.lock);
		if (cmd)
		{
			free(cmd);
			cmd = NULL;
		}
		data->cmd_count++;
		cmd = ft_get_next_cmd(data);
	}
}

int	ft_lstsize(t_result *lst)
{
	int	i;

	i = 0;
	while (lst)
	{
		lst = lst->next;
		i++;
	}
	return (i);
}

void	ft_printf_list(t_result *list)
{
	t_result	*tmp;

	tmp = list;
	while(list)
	{
		ft_write_fd(1, list->result);
		if (list == list->next)
			exit (1);
		list = list->next;
	}
	ft_write_fd_nl(1, ft_int_to_string(ft_lstsize(tmp)));
}

void	ft_delete_elem(t_result **first, t_result *to_del)
{
	t_result	*tmp;

	if (!*first || !to_del)
		return ;
	tmp = *first;
	while (tmp && tmp != to_del && tmp->next != to_del)
		tmp = tmp->next;
	if (to_del->result)
		free(to_del->result);
	to_del->result = NULL;
	if (*first == to_del)
		*first = to_del->next;
	else
		tmp->next = to_del->next;
	free(to_del);
	to_del = NULL;
}

int	ft_compare_inside_list(t_data *data)
{
	//ft_write_fd(1, "compare\n");
	//ft_write_fd(1, data->bash_read->result);
	//ft_write_fd(1, data->mini_read->result);
	if (!data->bash_read || !data->mini_read)
		return (0);
	if (ft_strcmp(data->bash_read->result, data->mini_read->result) == 1)
		return (1);
	if (ft_is_in_list(data) == 1)
		return (1);
	return (0);
}

int	ft_is_in_list(t_data *data)
{
	t_result	*ptr_mini;
	t_result	*best_fit;
	int			best_num;
	int			actual_num;

	ptr_mini = data->mini_read;
	//ft_write_fd_nl(1, "inside is in list");
	while (ptr_mini)
	{
		if (ft_strcmp(data->bash_read->result, ptr_mini->result) == 1)
		{
			/*
			ft_printf_list(data->mini.read_result);
			ft_write_fd(1, "going to swap!\n");
			ft_write_fd_nl(1, COLOR_YELLOW"before swap!");
			ft_write_fd(1, data->mini_read->result);
			ft_write_fd_nl(1, ptr_mini->result);
			ft_write_fd(1, COLOR_DEFAULT);*/
			ft_swap_elements(&data->mini.read_result, data->mini_read, ptr_mini);
			data->mini_read = ptr_mini;
			//ft_write_fd(1, "after swap!\n");
			//ft_delete_elem(&result_mini, ptr_mini);
			//ft_printf_list(data->mini.read_result);
			return (1);
		}
		ptr_mini = ptr_mini->next;
	}
	best_fit = data->mini_read;
	ptr_mini = data->mini_read;
	best_num = 0;
	while (ptr_mini)
	{
		actual_num = ft_chars_equal(data->bash_read->result, ptr_mini->result);
		if (actual_num > best_num)
		{
			best_num = actual_num;
			best_fit = ptr_mini;
		}
		ptr_mini = ptr_mini->next;
	}
	ft_swap_elements(&data->mini.read_result, data->mini_read, best_fit);
	data->mini_read = best_fit;
	return (0);
}

int	ft_chars_equal(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1 && s2)
	{
		if (s1[i] != s2[i])
			return (i);
		i++;
	}
	return (i);
}

void	ft_swap_elements(t_result **first, t_result *swap_a, t_result *swap_b)
{
	t_result	*before_a;
	t_result	*before_b;
	t_result	*tmp;

	before_a = *first;
	before_b = *first;
	if (swap_a == swap_b)
		return ;
	while (before_a && before_a->next != swap_a)
		before_a = before_a->next;
	while (before_b && before_b->next != swap_b)
		before_b = before_b->next;
	if (before_a)
	{
		tmp = before_a->next;
		before_a->next = swap_b;
	}
	else
		*first = swap_b;
	if (before_b != swap_a)
	{
		before_b->next = swap_a;
		tmp = swap_a->next;
	}
	else
		tmp = swap_a;
	swap_a->next = swap_b->next;
	swap_b->next = tmp;
	
	
	//ft_write_fd(1, "swaped ");
	//ft_write_fd(1, swap_a->result);
	//ft_write_fd(1, swap_b->result);
}

void	ft_compare_result(t_data *data)
{
	char	*tmp_bash;
	char	*tmp_mini;
	int		result;

	data->bash_read = data->bash.read_result;
	data->mini_read = data->mini.read_result;
	while(data->bash_read || data->mini_read)
	{
		if (data->bash_read && data->mini_read)
			result = ft_compare_inside_list(data);//ft_strcmp(result_bash->result, result_mini->result);
		else if (!data->bash_read && !data->mini_read)
			result = 1;
		else
			result = 0;
		if (result == 1)
		{
			data->tests_passed++;
			ft_write_fd(1, COLOR_GREEN"[OK]"COLOR_DEFAULT);
			/*ft_write_fd(1, COLOR_GREEN);
			ft_write_fd(1, data->bash_read->result);
			ft_write_fd(1, data->mini_read->result);
			ft_write_fd(1, COLOR_DEFAULT);*/
		}
		else
		{
			ft_write_fd(1, "\nresult bash     =["COLOR_BLUE);
			if (data->bash_read)
				tmp_bash = ft_replace_new_line(data->bash_read->result);
			if (data->mini_read)
				tmp_mini = ft_replace_new_line(data->mini_read->result);
			ft_write_fd_compared(1, tmp_bash, tmp_mini);
			//ft_write_fd(1, tmp);
			ft_write_fd(1, COLOR_DEFAULT"]\nresult minishell=["COLOR_BLUE);
			ft_write_fd_compared(1, tmp_mini, tmp_bash);
			//ft_write_fd(1, tmp);
			if (tmp_bash)
				free(tmp_bash);
			if (tmp_mini)
				free(tmp_mini);
			tmp_bash = NULL;
			tmp_mini = NULL;
			ft_write_fd(1, COLOR_DEFAULT"]"COLOR_RED"[KO]\n"COLOR_DEFAULT);
			data->tests_failed++;
		}
		data->test_count++;
		if (data->mini_read)
			data->mini_read = data->mini_read->next;
		if (data->bash_read)
			data->bash_read = data->bash_read->next;
	}
}

void	ft_write_fd_compared(int fd, char *s1, char *s2)
{
	int	i;
	int	s1_len;

	s1_len = ft_strlen(s1);
	i = 0;
	while (s1 && s2)
	{
		if (s1[i] != s2[i])
			break ;
		i++;
	}
	write(fd, s1, i);
	if (i != s1_len)
	{
		ft_write_fd(1, COLOR_RED);
		ft_write_fd(fd, &s1[i]);
	}
}

char	*ft_get_bash_path(t_data *data)
{
	char	**paths;
	char	*output;

	paths = ft_split(ft_getenv("PATH", data->envp), ':');
	if (!paths)
		paths = ft_split(DEFAULT_PATH, ':');
	output = ft_check_path("bash", paths);
	ft_free_char_array(paths);
	return (output);
}

void	ft_alloc_mem_and_assign(t_data *data, char **argv, char **envp)
{
	data->argv = argv;
	data->envp = envp;
	data->mini_path = ft_realloc(argv[1], "/minishell", 0, 0);
	data->cmd_count = 0;
	data->test_count = 0;
	data->tests_passed = 0;
	data->tests_failed = 0;
	data->mini.read_result = NULL;
	data->bash.read_result = NULL;
}
