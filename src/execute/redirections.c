/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbiederm <pbiederm@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/10 18:30:01 by pbiederm          #+#    #+#             */
/*   Updated: 2022/12/11 13:06:45 by pbiederm         ###   ########.fr       */
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


// while(elements->in_f != NULL && elements->in_f[number_of_infiles].type == DELIMITOR)
// {
// 	elements->fd_in = dup(here_doc(elements->in_f[number_of_infiles].name));
// 	number_of_infiles++;
// }
// number_of_infiles = 0;
void	redirect_in(t_chunk **salt, t_vars *vars)
{
	t_chunk	*element;

	element = *salt;

	if(in_redirection_this_node(&element))
	{
		while (element->in_f != NULL &&
		element->in_f[vars->number_of_infiles].name != NULL &&
		((element->in_f[vars->number_of_infiles].type == INPUT_F) ||
		(element->in_f[vars->number_of_infiles].type == DELIMITOR)))
		{
			if(element->in_f[vars->number_of_infiles].type == INPUT_F)
			{
				element->fd[0] = open(element->in_f[vars->number_of_infiles].name, O_RDONLY);
			}
			else if(element->in_f[vars->number_of_infiles].type == DELIMITOR)
			{
				element->fd[0] = here_doc(element->in_f[vars->number_of_infiles].name);
			}
			vars->number_of_infiles++;
		}
		vars->number_of_infiles = 0;
		dup2(element->fd[0], STDIN_FILENO);
	}
}

void	redirect_out(t_chunk **salt, t_vars *vars)
{
	t_chunk	*element;
	
	element = *salt;
	if(out_redirection_this_node(&element))
	{
		while (element->out_f != NULL &&
		element->out_f[vars->number_of_outfiles].name != NULL &&
		((element->out_f[vars->number_of_outfiles].type == OUTPUT_F)||
		(element->out_f[vars->number_of_outfiles].type == R_AP_OUTPUT_F)))
		{	
			if(element->out_f[vars->number_of_outfiles].type == OUTPUT_F)
			{
				element->fd[1] = open(element->out_f[vars->number_of_outfiles].name, \
				O_WRONLY | O_CREAT | O_TRUNC, 0664);
			}
			else if(element->out_f[vars->number_of_outfiles].type == R_AP_OUTPUT_F)
			{
				element->fd[1] = open(element->out_f[vars->number_of_outfiles].name, \
				O_WRONLY | O_CREAT | O_APPEND, 0664);
			}
			vars->number_of_outfiles++;
		}
		vars->number_of_outfiles = 0;
		dup2(element->fd[1], STDOUT_FILENO);
	}
}

void	redirect_io(t_chunk **salt, t_vars *vars)
{
	t_chunk	*element;
	
	element = *salt;
	if (in_redirection_this_node(&element) && out_redirection_this_node(&element))
	{
		while (element->in_f != NULL && element->in_f[vars->number_of_infiles].name != NULL && element->in_f[vars->number_of_infiles].type == INPUT_F)
		{
			element->fd[0] = open(element->in_f[vars->number_of_infiles].name, O_RDONLY);
			vars->number_of_infiles++;
		}
		vars->number_of_infiles = 0;
		while (element->out_f != NULL &&
		element->out_f[vars->number_of_outfiles].name != NULL &&
		((element->out_f[vars->number_of_outfiles].type == OUTPUT_F)||
		(element->out_f[vars->number_of_outfiles].type == R_AP_OUTPUT_F)))
		{	
			if(element->out_f[vars->number_of_outfiles].type == OUTPUT_F)
			{
				element->fd[1] = open(element->out_f[vars->number_of_outfiles].name, \
				O_WRONLY | O_CREAT | O_TRUNC, 0664);
			}
			else if(element->out_f[vars->number_of_outfiles].type == R_AP_OUTPUT_F)
			{
				element->fd[1] = open(element->out_f[vars->number_of_outfiles].name, \
				O_WRONLY | O_CREAT | O_APPEND, 0664);
			}
			vars->number_of_outfiles++;
		}
		vars->number_of_outfiles = 0;
		dup2(element->fd[1], STDOUT_FILENO);
		dup2(element->fd[0], STDIN_FILENO);
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