/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_fork.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbiederm <pbiederm@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/27 11:52:18 by pbiederm          #+#    #+#             */
/*   Updated: 2022/12/09 20:46:41 by pbiederm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*need a simple command execute, execute one command*/
#include	"../../includes/minishell.h"

void	get_exit_status(t_vars *vars,  int status);
int		count_command_number(t_chunk **salt);
t_vars	*initialize_vars(t_chunk **salt);
int		pipe_this_node(t_chunk **salt);
int		in_redirection_this_node(t_chunk **salt);
int		pipe_last_node(t_chunk **salt);


t_vars	*initialize_vars(t_chunk **salt)
{
	t_vars	*vars;
	
	vars = malloc(sizeof(*vars));
	vars->num_cmd = count_command_number(salt);
	vars->number_of_infiles = 0;
	vars->number_of_outfiles = 0;
	return(vars);
}

int	count_command_number(t_chunk **salt)
{
	t_chunk	*elements;
	int		num_cmds;
	
	elements = *salt;
	num_cmds = 0;
	while(elements)
	{
		if(elements->indentifier == CMD_BLOCK)
			num_cmds++;
		elements = elements->next;
	}
	return(num_cmds);
}

int	pipe_this_node(t_chunk **salt)
{
	t_chunk	*elements;
	
	elements = *salt;
	if(elements->next != NULL && elements->out_f == NULL && elements->next->in_f == NULL)
		return(1);
	else
		return(0);
}

int	pipe_last_node(t_chunk **salt)
{
	t_chunk	*elements;
	
	elements = *salt;
	if(elements->prev != NULL && elements->prev->out_f == NULL && elements->in_f == NULL)
		return(1);
	else
		return(0);
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

// void open_infiles(t_chunk **salt, t_vars *vars)
// {
// 	t_chunk	*elements;

// 	elements = *salt; 
// 	while (elements->in_f != NULL && elements->in_f[vars->number_of_infiles].name != NULL && elements->in_f[vars->number_of_infiles].type == INPUT_F)
// 	{
// 		fd[i][0] = open(elements->in_f[vars->number_of_infiles].name, O_RDONLY);
// 		vars->number_of_infiles++;
// 	}
// 	vars->number_of_infiles = 0;
// }

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

void	execute(t_chunk **salt, t_info *info, char	**envp)
{
	t_chunk	*elements;
	t_vars	*vars;

	int		pids;
	int		i;
	int		status;
	int		save_std_out;
	int		save_std_in;
	int		fd[9999][2];
	// int		pipe_fd[2];
	// int		read_from_pipe;
 
	// pipe(pipe_fd);
	// simple_execute(salt, info, envp);
	elements = *salt;
	vars = initialize_vars(salt);
	i = 0;
	while (elements)
	{
		save_std_out = dup(STDOUT_FILENO);
		save_std_in = dup(STDIN_FILENO);
		fd[i][0] = save_std_in;
		fd[i][1] = save_std_out;
		if (pipe_this_node(&elements))
		{
			pipe(fd[i]);
		}
		if (elements->indentifier == CMD_BLOCK && elements->command_path != NULL)
		{
			pids = fork();
			if (pids == -1)
			{
				freeing_chunks(salt, info);
				free(vars);
				write(2, "Error while creating process\n", 30);
			}
			if (pids == 0)
			{
				if (i != vars->num_cmd - 1 && pipe_this_node(&elements))
				{
					dup2(fd[i][1], STDOUT_FILENO);
					close(fd[i][0]);
					close(fd[i][1]);
				}
				if (i != 0 && i != vars->num_cmd - 1 && pipe_last_node(&elements))
				{
					dup2(fd[i - 1][0], STDIN_FILENO);
					close(fd[i - 1][1]);
					close(fd[i - 1][0]);
				}
				if (out_redirection_this_node(&elements))
				{
					while (elements->out_f != NULL && elements->out_f[vars->number_of_outfiles].name != NULL && elements->out_f[vars->number_of_outfiles].type == OUTPUT_F)
					{
						fd[i][1] = open(elements->out_f[vars->number_of_outfiles].name, O_WRONLY | O_CREAT | O_TRUNC, 0664);
						vars->number_of_outfiles++;
					}
					
					vars->number_of_outfiles = 0;
					dup2(fd[i][1], STDOUT_FILENO);
					close(fd[i][0]);
					close(fd[i][1]);
				}
				if (in_redirection_this_node(&elements))
				{
					while (elements->in_f != NULL && elements->in_f[vars->number_of_infiles].name != NULL && elements->in_f[vars->number_of_infiles].type == INPUT_F)
					{
						fd[i][0] = open(elements->in_f[vars->number_of_infiles].name, O_RDONLY);
						vars->number_of_infiles++;
					}
					vars->number_of_infiles = 0;
					dup2(fd[i][0], STDIN_FILENO);
					close(fd[i][1]);
					close(fd[i][0]);
					if (i == vars->num_cmd - 1)
					{
						dup2(zero_fd[0], STDIN_FILENO);
						close(zero_fd[0]);
					}
				}
				run(elements, info, envp);
			}
		}
		if (i != 0)
		{
			close(fd[i - 1][0]);
			close(fd[i - 1][1]);
		}
		close(zero_fd[1]);
		close(zero_fd[0]);
		dup2(save_std_in, STDIN_FILENO);
		close(save_std_in);
		dup2(save_std_out, STDOUT_FILENO);
		close(save_std_out);
		waitpid(-1, &status, 0);
		get_exit_status(vars, status);
		i++;
		elements = elements->next;
	}
}

	// 	// If a command is not found, the child process created to execute it 
	// 	// returns a status of 127. If a command is found but is not executable, 
	// 	// the return status is 126.
	// 	fprintf(stderr,"exit status: %d\n", g_exit_status);
	// 	fprintf(stderr, "Parrent waited for process pids[%d]\n", i);
void	get_exit_status(t_vars *vars,  int status)
{
	int	i;

	i = 0;
	while(i < vars->num_cmd)
	{
		g_exit_status = WEXITSTATUS(status);
		i++;
	}
}

// execute_enchanced(t_chunk **salt, t_info *info, char	**envp)
// {
// 	int fd[10][2],i,pc;
// 	char *argv[100];

// 	for(i=0;i<nr;i++){
// 		tokenize_buffer(argv,&pc,buf[i]," ");
// 		if(i!=nr-1){
// 			if(pipe(fd[i])<0){
// 				perror("pipe creating was not successfull\n");
// 				return;
// 			}
// 		}
// 		if(fork()==0){//child1
// 			if(i!=nr-1){
// 				dup2(fd[i][1],1);
// 				close(fd[i][0]);
// 				close(fd[i][1]);
// 			}

// 			if(i!=0){
// 				dup2(fd[i-1][0],0);
// 				close(fd[i-1][1]);
// 				close(fd[i-1][0]);
// 			}
// 			execvp(argv[0],argv);
// 			perror("invalid input ");
// 			exit(1);//in case exec is not successfull, exit
// 		}
// 		//parent
// 		if(i!=0){//second process
// 			close(fd[i-1][0]);
// 			close(fd[i-1][1]);
// 		}
// 		waitpid(NULL);
// 	}

// }
					
// if (i == 0)
		// {
			
		// 	while(infile_in_this_node(&elements, &vars))
		// 	{
		// 		fd[i][0] = open(elements->in_f[vars->number_of_infiles].name, O_RDONLY);
		// 		vars->number_of_infiles++;
		// 	}
		// 	vars->number_of_infiles = 0;
		// 	while(elements->in_f != NULL && elements->in_f[vars->number_of_infiles].type == DELIMITOR)
		// 	{
		// 		fd[i][0] = dup(here_doc(elements->in_f[vars->number_of_infiles].name));
		// 		vars->number_of_infiles++;
		// 	}
		// 	vars->number_of_infiles = 0;
		// }
		// else
		// {
		// 	while(elements->in_f != NULL && elements->in_f[vars->number_of_infiles].name != NULL && elements->in_f[vars->number_of_infiles].type == INPUT_F)
		// 	{
		// 		fd[i][0] = open(elements->in_f[vars->number_of_infiles].name, O_RDONLY);
		// 		vars->number_of_infiles++;
		// 	}
		// 	vars->number_of_infiles = 0;
		// 	while(elements->in_f != NULL && elements->in_f[vars->number_of_infiles].type == DELIMITOR)
		// 	{
		// 		fd[i][0] = dup(here_doc(elements->in_f[vars->number_of_infiles].name));
		// 		vars->number_of_infiles++;
		// 	}
		// 	vars->number_of_infiles = 0;
		// }
		// if (i == vars->num_cmd - 1)
		// {
		// 	while(elements->out_f != NULL && elements->out_f[vars->number_of_outfiles].name != NULL && elements->out_f[vars->number_of_outfiles].type == OUTPUT_F)
		// 	{
		// 		elements->fd_out = open(elements->out_f[vars->number_of_outfiles].name, O_WRONLY | O_CREAT | O_TRUNC, 0664);
		// 		vars->number_of_outfiles++;
		// 	}
		// 	vars->number_of_outfiles = 0;
		// 	while(elements->out_f != NULL && elements->out_f[vars->number_of_outfiles].name != NULL && elements->out_f[vars->number_of_outfiles].type == R_AP_OUTPUT_F)
		// 	{
		// 		elements->fd_out = open(elements->out_f[vars->number_of_outfiles].name, O_WRONLY | O_CREAT | O_APPEND, 0664);
		// 		vars->number_of_outfiles++;
		// 	}
		// 	vars->number_of_outfiles = 0;
		// }
		// else
		// {
		// 	/*place for all output except for last one*/
		// 	while(elements->out_f != NULL && elements->out_f[vars->number_of_outfiles].name != NULL && elements->out_f[vars->number_of_outfiles].type == OUTPUT_F)
		// 	{
		// 		elements->fd_out = open(elements->out_f[vars->number_of_outfiles].name, O_WRONLY | O_CREAT | O_TRUNC, 0664);
		// 		vars->number_of_outfiles++;
		// 	}
		// 	vars->number_of_infiles = 0;
		// 	while(elements->out_f != NULL && elements->out_f[vars->number_of_outfiles].name != NULL && elements->out_f[vars->number_of_outfiles].type == R_AP_OUTPUT_F)
		// 	{
		// 		elements->fd_out = open(elements->out_f[vars->number_of_outfiles].name, O_WRONLY | O_CREAT | O_APPEND, 0664);
		// 		vars->number_of_outfiles++;
		// 	}
		// 	vars->number_of_outfiles = 0;
		// }