/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkavalia <lkavalia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 12:37:21 by lkavalia          #+#    #+#             */
/*   Updated: 2022/12/23 19:35:28 by lkavalia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_collect	g_errors;

static void	check_before_specials(t_token **token)
{
	if ((*token)->t[0] == '|')
		(*token)->name = PIPE;
	else if ((*token)->t[0] == '<' && (*token)->t[1] != '<')
		(*token)->name = R_INPUT;
	else if ((*token)->t[0] == '<' && (*token)->t[1] == '<')
		(*token)->name = R_AP_INPUT;
	else if ((*token)->t[0] == '>' && (*token)->t[1] != '>')
		(*token)->name = R_OUTPUT;
	else if ((*token)->t[0] == '>' && (*token)->t[1] == '>')
		(*token)->name = R_AP_OUTPUT;
}

static void	initialize_hive(t_data *h, char **envp)
{
	h->env = NULL;
	h->exp_l = NULL;
	h->c_arr = NULL;
	h->token = NULL;
	create_e_list(&h->env, envp);
	create_e_list(&h->exp_l, envp);
	g_errors.bip = false;
}

static void	recognise_specials(t_token **token)
{
	t_token	*temp;

	temp = (*token);
	while ((*token) != NULL)
	{
		check_before_specials(token);
		(*token) = (*token)->next;
	}
	(*token) = temp;
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
		recognise_specials(&hive->token);
		register_tokens(&hive->info, &hive->token, hive->env);
		get_the_commands(hive->token, hive->env, &hive->c_arr, &hive->info);
		check_for_executables(&hive->c_arr);
		execute(&hive->c_arr, hive, envp);
		freeing_tokens(hive->token);
		freeing_chunks(&hive->c_arr);
	}
	if (ft_strlen(hive->info.r) != 0)
		add_history(hive->info.r);
	free(hive->info.r);
}

int	main(int argc, char **argv, char **envp)
{
	t_data	hive;

	if (argc != 1)
		return (1);
	(void)argv;
	g_errors.g_exit_status = 0;
	initialize_hive(&hive, envp);
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		hive.info.r = readline("Juno> ");
		if (readline_err(hive) == 1)
			break ;
		initialize_info(&hive.info);
		errors_before(&hive.info);
		parsing_and_execution(&hive, envp);
	}
	free(hive.info.r);
	rl_clear_history();
	freeing_e_list(&(&hive)->env);
	freeing_e_list(&(&hive)->exp_l);
	return (0);
}
