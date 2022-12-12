/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbiederm <pbiederm@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/10 18:30:01 by pbiederm          #+#    #+#             */
/*   Updated: 2022/12/12 21:02:12 by pbiederm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"../../includes/minishell.h"

int out_redirection_this_node(t_chunk **salt)
{
	t_chunk *element;

	element = *salt;
	if (element->out_f != NULL)
	{
		return(1);
	}
	else
	{
		return(0);
	}
}

void	redirect_in(t_chunk **salt, t_vars *vars)
{
	t_chunk	*element;
	int	input_fd;

	input_fd = -11;

	element = *salt;

	if(in_redirection_this_node(&element))
	{
		while (element->in_f != NULL &&
		element->in_f[vars->number_of_infiles].name != NULL)
		{
			if(element->in_f[vars->number_of_infiles].type == INPUT_F)
			{
				input_fd = open(element->in_f[vars->number_of_infiles].name, O_RDONLY);
				if(input_fd == -1)
				{
					input_fd = open("./includes/err_read.txt", O_RDONLY|O_CREAT, 0644);
					write(2,"Go outside, take a walk, rethink inputing nonexisting files.\n", 62);
					g_exit_status = 1;
				}
			}
			else if(element->in_f[vars->number_of_infiles].type == DELIMITOR)
			{
				input_fd = here_doc(element->in_f[vars->number_of_infiles].name);
			}
			vars->number_of_infiles++;
		}
		vars->number_of_infiles = 0;
		dup2(input_fd, STDIN_FILENO);
		close(input_fd);
	}
}

void	redirect_out(t_chunk **salt, t_vars *vars)
{
	t_chunk	*element;
	int		output_fd;
	
	element = *salt;
	if(out_redirection_this_node(&element))
	{
		while (element->out_f != NULL &&
		element->out_f[vars->number_of_outfiles].name != NULL)
		{	
			if(element->out_f[vars->number_of_outfiles].type == OUTPUT_F)
			{
				output_fd = open(element->out_f[vars->number_of_outfiles].name, \
				O_WRONLY | O_CREAT | O_TRUNC, 0664);
				if(output_fd == -1)
				{
					write(2,"Could not create output file.\n", 31);
					g_exit_status = 1;
				}
			}
			else if(element->out_f[vars->number_of_outfiles].type == R_AP_OUTPUT_F)
			{
				output_fd = open(element->out_f[vars->number_of_outfiles].name, \
				O_WRONLY | O_CREAT | O_APPEND, 0664);
				if(output_fd == -1)
				{
					write(2,"Could not create output append file.\n", 38);
					g_exit_status = 1;
				}
			}
			vars->number_of_outfiles++;
		}
		vars->number_of_outfiles = 0;
		dup2(output_fd, STDOUT_FILENO);
		close(output_fd);
	}
}

void	redirect_io(t_chunk **salt, t_vars *vars)
{
	t_chunk	*element;
	int		output_fd;
	int		input_fd;
	
	element = *salt;
	if (in_redirection_this_node(&element) && out_redirection_this_node(&element))
	{
		while (element->in_f != NULL && element->in_f[vars->number_of_infiles].name != NULL && element->in_f[vars->number_of_infiles].type == INPUT_F)
		{
			if(element->in_f[vars->number_of_infiles].type == INPUT_F)
			{
				input_fd = open(element->in_f[vars->number_of_infiles].name, O_RDONLY);
				if(input_fd == -1)
				{
					input_fd = open("./includes/err_read.txt", O_RDONLY|O_CREAT, 0644);
					write(2,"Could not create output append file.\n", 38);
					g_exit_status = 1;
				}
			}
			else if(element->in_f[vars->number_of_infiles].type == DELIMITOR)
			{
				input_fd = here_doc(element->in_f[vars->number_of_infiles].name);
			}
			vars->number_of_infiles++;
		}
		vars->number_of_infiles = 0;
		while (element->out_f != NULL &&
		element->out_f[vars->number_of_outfiles].name != NULL)
		{	
			if(element->out_f[vars->number_of_outfiles].type == OUTPUT_F)
			{
				output_fd = open(element->out_f[vars->number_of_outfiles].name, \
				O_WRONLY | O_CREAT | O_TRUNC, 0664);
				if (output_fd == -1)
				{
					g_exit_status = 1;
					write(2, "Invalid output file.\n", 22);
				}
			}
			else if(element->out_f[vars->number_of_outfiles].type == R_AP_OUTPUT_F)
			{
				output_fd = open(element->out_f[vars->number_of_outfiles].name, \
				O_WRONLY | O_CREAT | O_APPEND, 0664);
				if (output_fd == -1)
				{
					g_exit_status = 1;
					write(2, "Invalid output append file.\n", 22);
				}
			}
			vars->number_of_outfiles++;
		}
		vars->number_of_outfiles = 0;
		dup2(output_fd, STDOUT_FILENO);
		dup2(input_fd, STDIN_FILENO);
		close(output_fd);
		close(input_fd);
	}
}

int in_redirection_this_node(t_chunk **salt)
{
	t_chunk *element;

	element = *salt;
	if(element->in_f != NULL)
	{
		return(1);
	}
	else
	{
		return(0);
	}
}

int in_redirection_next_node(t_chunk **salt)
{
	t_chunk *element;

	element = *salt;
	if(element->next != NULL && element->next->in_f != NULL)
	{
		return(1);
	}
	else
	{
		return(0);
	}
}