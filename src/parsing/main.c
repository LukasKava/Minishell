/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbiederm <pbiederm@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 12:37:21 by lkavalia          #+#    #+#             */
/*   Updated: 2022/12/01 16:38:25 by pbiederm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../includes/minishell.h"

static void check_non_generic(t_info *info)
{
	int i;

	i = 0;
	if (ft_strlen(info->readline) > 0)
	{
		while (info->readline[i] == ' ' && info->readline[i] != '\0')
			i++;
		if (info->readline[i] == '&' && info->readline[i + 1] == '&')
		{
			printf("bash: syntax error near unexpected token `&&'\n");
			info->error = true;
			info->exit_status = 2;
			return;
		}
		if (info->readline[i] == ';' && info->readline[i + 1] == ';')
		{
			printf("bash: syntax error near unexpected token `;;'\n");
			info->error = true;
			info->exit_status = 2;
			return;
		}
		if (info->readline[i] == ';' && info->readline[i + 1] != ';')
		{
			printf("bash: syntax error near unexpected token `;'\n");
			info->error = true;
			info->exit_status = 2;
			return;
		}
		if (info->readline[i] == '&' && info->readline[i + 1] != '&')
		{
			printf("bash: syntax error near unexpected token `&'\n");
			info->error = true;
			info->exit_status = 2;
			return;
		}
		if (info->readline[i] == '(' || info->readline[i] == ')')
		{
			printf("in\n");
			i++;
			while (info->readline[i] == ' ' && info->readline[i] != '\0')
				i++;
			if (i > 0)
				i--;
			if (info->readline[i + 1] == '(' || info->readline[i + 1] == ')')
			{
				printf("bash: syntax error near unexpected token `)'\n");
				info->error = true;
				info->exit_status = 2;
				return;
			}
		}
	}
}

void errors_before(t_info *info)
{
	count_quotes(info);								// COUNTS THE QUOTES
	if (info->d_quotes != 0 || info->s_quotes != 0) // quotes not closed stops right here
	{
		if (check_quotes(info) == 1)
		{
			info->error = true;
			printf("Quotes are not closed!\n");
		}
	}
	if (pipe_excistence(info) == 0) // Checks if there are pipes and there are no problems with them.
	{
		if (pipe_cases(info) == 1)
		{
			info->error = true;
			printf("Syntax error | \n");
		}
	}
	check_non_generic(info);
	check_dollar_signs(info);
}

int main(int argc, char **argv, char **envp)
{
	t_info info;
	t_token *token;
	t_chunk	*chunk_array;
	//t_env	*e_l;
	//t_env	*exp_l;

	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	token = NULL;
	chunk_array = NULL;
	//e_l = NULL;
	//exp_l = NULL;
	if (argc != 1)
	{
		printf("%s doesn't need more arguments.\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	//create_e_list(&e_l, envp);
	//create_e_list(&exp_l, envp);
	while (1)
	{
		info.readline = readline("Mini_hell\U0001F34C\U0001F412> ");
		if (!info.readline)
		{
			write(1, "\033[0;31mCtrl-D was activated\033[0m\n", 33);
			break ;
		}
		initialize_info(&info);
		errors_before(&info);
		if (info.error == false)
		{
			//PARSING
			token = initialize_token(token, &info);
			chunk_array = initialize_chunk(chunk_array, &info);
			lexer(&info, &token);
			print_the_list("after lexing", token);
			register_tokens(&info, &token, envp);
			print_the_list("register tokens check", token);
			get_the_commands(&info, token, envp, &chunk_array);
			if (info.error == false)
			{
				print_the_chunk_list("CHUNK LIST", chunk_array);
			//	print_the_chunk_list_backwards("CHUNK LIST BACWARDS", chunk_array);
			}
			//EXECUTION CAN BEGIN
			execute(&chunk_array, &info, envp);
			//second_child(&chunk_array, &info, envp);
			// int number_of_infiles = 0;
			// int number_of_outfiles = 0;
		    // while(chunk_array)
			// {
			// 	while(chunk_array->in_f != NULL && chunk_array->in_f[number_of_infiles].name != NULL)
			// 	{
		
			// 		fprintf(stderr, "string of files [%d]: %s\n", number_of_infiles, chunk_array->in_f[number_of_infiles].name);
			// 		number_of_infiles++;
			// 	}
			// 	while(chunk_array->out_f != NULL && chunk_array->out_f[number_of_outfiles].name != NULL)
			// 	{
			// 		fprintf(stderr, "string of files [%d]: %s\n", number_of_outfiles, chunk_array->out_f[number_of_outfiles].name);
			// 		number_of_outfiles++;
			// 	}
			// 	fprintf(stderr,"command path : %s\n", chunk_array->command_path);
				// fprintf(stderr,"command argument : %s\n", chunk_array->arguments);
				// number_of_infiles = 0;
				// number_of_outfiles = 0;
				// chunk_array->fd_in;
				// chunk_array->fd_out;
			// 	chunk_array = chunk_array->next;
			// }
			freeing_tokens(token);
			freeing_chunks(&chunk_array, &info);
		}
		if (ft_strlen(info.readline) != 0)
			add_history(info.readline);
		free(info.readline);
	}
	//freeing_e_list(e_l);
	//freeing_e_list(exp_l);
	return (0);
}
