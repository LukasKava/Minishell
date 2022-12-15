/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_tester.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkaufman <rkaufman@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/01 21:49:02 by rkaufman          #+#    #+#             */
/*   Updated: 2022/04/04 20:24:42 by rkaufman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef MINISHELL_TESTER_H
# define MINISHELL_TESTER_H
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <pthread.h>
# include <fcntl.h>
# include <signal.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/wait.h>
# define FILE_RIGHTS 0744
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1024
# endif
# ifndef DEFAULT_PATH
#  define DEFAULT_PATH "/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin"
# endif
# define COLOR_DEFAULT "\033[0m"
# define COLOR_RED "\033[31m"
# define COLOR_GREEN "\033[32m"
# define COLOR_YELLOW "\033[33m"
# define COLOR_BLUE "\033[34m"
# define COLOR_MAGENTA "\033[35m"
# define COLOR_WHITE "\033[37m"
# ifndef MAX_FD
#  define MAX_FD 65536
# endif

typedef struct s_pipe {
	int			fd_read;
	int			fd_write;
}				t_pipe;

typedef struct s_result {
	char			*result;
	struct s_result	*next;
}				t_result;

typedef struct s_thread {
	pthread_t		thread_read;
	pthread_t		thread_error;
	pthread_mutex_t	lock;
	struct s_result	*read_result;
	struct s_result	*err_result;
}				t_thread;


typedef struct s_data {
	char			**argv;
	char			**envp;
	struct s_pipe	main_mini;
	struct s_pipe	main_bash;
	struct s_pipe	mini_main;
	struct s_pipe	bash_main;
	int				pid_mini;
	int				pid_bash;
	int				fd_tests;
	int				cmd_count;
	int				test_count;
	int				tests_passed;
	int				tests_failed;
	char			*test_cmd;
	char			*mini_path;
	char			*bash_path;
	char			*mini_prompt;
	char			*bash_prompt;
	struct s_thread	mini;
	struct s_result	*mini_read;
	struct s_thread	bash;
	struct s_result	*bash_read;
}				t_data;

//ft_tester.c
void			ft_alloc_mem_and_assign(t_data *data, char **argv, char **envp);
char			*ft_get_bash_path(t_data *data);
void			ft_test_loop(t_data *data);
void			ft_compare_result(t_data *data);
void			ft_write_summary(t_data *data);
void			ft_delete_elem(t_result **first, t_result *to_del);
int				ft_compare_inside_list(t_data *data);
int				ft_is_in_list(t_data *data);
void			ft_write_fd_compared(int fd, char *s1, char *s2);
void			ft_swap_elements(t_result **first, t_result *swap_a, t_result *swap_b);
void			ft_printf_list(t_result *list);
int				ft_chars_equal(char *s1, char *s2);

//ft_utils0.c
int				ft_str_var_cmp(char *var_name, char *var_elem);
size_t			ft_strlen(const char *s);
void			*ft_memset(void *s, int c, size_t n);
size_t			ft_copy(char *dst, char *src, size_t size);
char			*ft_string_dup(const char *s);
char			*ft_get_substring(char const *s, size_t start, size_t len);
char			*ft_int_to_string(long n);
size_t			ft_get_int_len(long n, int sign);
size_t			ft_count_of_in_str(const char *s, const char c);
char			*ft_char_in_str(const char *s, const char c);
char			*ft_realloc(char *s1, char *s2, int free_s1, int free_s2);
int				ft_pos_in_string(char *s, char c);
int				ft_strcmp(const char *s1, const char *s2);
int				ft_strncmp(const char *s1, const char *s2, size_t n);
char			*ft_strnstr(const char *big, const char *little, size_t len);
char			*ft_remove_char(char *s, char c);
char			*ft_check_path(char *cmd, char **paths);
char			*ft_get_next_cmd(t_data *data);
char			*ft_replace_new_line(char *input);
void			ft_write_test_cmd_to(char *cmd, char *to);
int				ft_write_err(char *output);
char			*ft_getenv(char *var_name, char **envp);
void			ft_write_fd(int fd, char *s);
void			ft_write_fd_nl(int fd, char *s);

//ft_split.c
char			**ft_split(char const *s, const char c);
size_t			ft_words_in_str(char const *s, const char c);
void			ft_free_char_array(char **array);

//ft_get_next_line.c
char			*ft_get_next_line(int fd);
char			*ft_read_file(int fd);

//ft_threads.c
int				ft_create_threads(t_data *data);
int				ft_destroy_threads(t_data *data);
void			*ft_thread_read_mini(void *input);
void			*ft_thread_read_mini(void *input);
void			*ft_thread_read_bash(void *input);

//ft_list.c
t_result		*ft_lstnew(char *content);
t_result		*ft_lstlast(t_result *lst);
void			ft_lstadd_back(t_result **lst, t_result *new);
void			ft_delete_list(t_result **lst);

//ft_communication.c
void			ft_send_to_minishell(t_data *data, char *cmd);
void			ft_send_to_bash(t_data *data, char *cmd);
char			*ft_read_from_minishell(t_data *data);
char			*ft_read_from_bash(t_data *data);
char			*ft_get_next_cmd(t_data *data);

//ft_children.c
void			ft_minishell_child(t_data *data);
void			ft_bash_child(t_data *data);

//ft_pipes.c
int				ft_create_pipes(t_data *data);
void			ft_close_main_pipes(t_data *data);
void			ft_close_pipes(t_data *data);

#endif
