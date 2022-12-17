/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbiederm <pbiederm@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 12:37:21 by lkavalia          #+#    #+#             */
/*   Updated: 2022/12/17 19:16:19 by pbiederm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../includes/minishell.h"

//int g_exit_status;
//bool bip = false;

t_collect g_errors;

// static void check_non_generic(t_info *info)
// {
// 	int i;

// 	i = 0;
// 	if (ft_strlen(info->readline) > 0)
// 	{
// 		while (info->readline[i] == ' ' && info->readline[i] != '\0')
// 			i++;
// 		if (info->readline[i] == '&' && info->readline[i + 1] == '&')
// 			simple_err_message(info, "syntax_err near unexpected token &&", 2);
// 		if (info->readline[i] == ';' && info->readline[i + 1] == ';')
// 			simple_err_message(info, "syntax_err near unexpected token ;;", 2);
// 		if (info->readline[i] == ';' && info->readline[i + 1] != ';')
// 			simple_err_message((info), "syntax_err: near unexpected token ;", 2);
// 		if (info->readline[i] == '&' && info->readline[i + 1] != '&')
// 			simple_err_message((info), "syntax_err: near unexpected token &", 2);
// 		if (info->readline[i] == '(' || info->readline[i] == ')')
// 		{
// 			printf("in\n");
// 			i++;
// 			while (info->readline[i] == ' ' && info->readline[i] != '\0')
// 				i++;
// 			if (i > 0)
// 				i--;
// 			if (info->readline[i + 1] == '(' || info->readline[i + 1] == ')')
// 				printf()
// 			{
// 				printf("bash: syntax error near unexpected token `)'\n");
// 				info->error = true;
// 				g_exit_status = 2;
// 				return ;
// 			}
// 		}
// 	}
// }

void errors_before(t_info *info)
{
	count_quotes(info);								// COUNTS THE QUOTES
	if (info->d_quotes != 0 || info->s_quotes != 0) // quotes not closed stops right here
	{
		if (check_quotes(info) == 1)
		{
			info->error = true;
			printf("Quotes are not closed!\n");
			g_errors.g_exit_status = 2;
			return ;
		}
	}
	if (pipe_excistence(info) == 0)
	{
		if (pipe_cases(info) == 1)
		{
			info->error = true;
			printf("Syntax error | \n");
			g_errors.g_exit_status = 2;
			return ;
		}
	}
//	check_non_generic(info);
	check_dollar_signs(info);
}

static void	initialize_hive(t_data *h, char **envp)
{
	h->env = NULL;
	h->exp_l = NULL;
	h->c_arr = NULL;
	h->token = NULL;
	create_e_list(&h->env, envp);
	create_e_list(&h->exp_l, envp);
	g_errors.g_exit_status = 0;
	g_errors.bip = false;
}

void	checker_before(t_data *hive)
{
	int i;

	i = 0;
	while (hive->info.readline[i] != '\0' && hive->info.readline[i] == ' ')
		i++;
	if (hive->info.readline[i] == '\0' || (hive->info.readline[i] >= '\a' && hive->info.readline[i] <= '\r'))
	{
			hive->info.error = true;
			g_errors.g_exit_status = 0;
	}
}

static void	parsing_and_execution(t_data *hive, char **envp)
{
	(void)envp;
	checker_before(hive);
	if (hive->info.error == false)
	{
		hive->token = initialize_token(hive->token, &hive->info);
		hive->c_arr = initialize_chunk(hive->c_arr, &hive->info);
		lexer(&hive->info, &hive->token);
		register_tokens(&hive->info, &hive->token, hive->env);
	//	print_the_list("register tokens check", hive->token);
		get_the_commands(hive->token, hive->env, &hive->c_arr, &hive->info);
		check_for_executables(&hive->c_arr);
		if (hive->info.error == false)
			print_the_chunk_list("CHUNK LIST", hive->c_arr);
		execute(&hive->c_arr, hive, envp);
		freeing_tokens(hive->token);
		freeing_chunks(&hive->c_arr);
	}
	if (ft_strlen(hive->info.readline) != 0)
		add_history(hive->info.readline);
	free(hive->info.readline);
}

//Command not found 127

int main(int argc, char **argv, char **envp)
{
	t_data	hive;

	if (argc != 1)
		return (1);
	(void)argv;
	initialize_hive(&hive, envp);
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		//signal(SIGINT, handle_sigint);
		hive.info.readline = readline("WeebOS> ");
		if (!hive.info.readline)
		{
			write(1, "\033[0;31mCtrl-D was activated a\033[0m\n", 35);
			break ;
		}
		initialize_info(&hive.info);
		errors_before(&hive.info);
		parsing_and_execution(&hive, envp);
	}
	free(hive.info.readline);
	rl_clear_history();
	freeing_e_list(&(&hive)->env);
	freeing_e_list(&(&hive)->exp_l);
	return (0);
}
