/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdenis <jdenis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/09 21:53:01 by dlacuey           #+#    #+#             */
/*   Updated: 2023/11/30 04:43:33 by jdenis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libft.h"
#include "execution.h"
#include "fcntl.h"
#include <sys/wait.h>
#include <stdio.h>
#include "colors.h"
#include "get_next_line.h"
#include <readline/readline.h>
#include "wildcards.h"
#include <sys/types.h>
#include <unistd.h>

extern char	**environ;
extern int	exit_status;

void	exec_in_the_son(t_node *node)
{
	char	**paths;
	char	*command;

	paths = find_paths(environ);
	if (!paths)
	{
		(exit_status = -1, perror(RED"No paths found"WHITE));
		return ;
	}
	command = get_command(node->vector_strs.values[0], paths);
	if (!command)
	{
		(exit_status = -1, free_strs(paths), perror(RED"Command not found"WHITE));
		return ;
	}
	wildcards(&(node->vector_strs.values));
	if (!node->vector_strs.values)
	{
		(exit_status = -1, free_strs(paths), perror(RED"Wildcards failed"));
		return ;
	}
	execve(command, node->vector_strs.values, environ);
	(exit_status = -1, free_strs(paths), free(command), perror(RED"Execve failed"WHITE));
	return ;
}

void	exec_simple_command(t_node *node)
{
	pid_t	pid1;

	pid1 = fork();
	if (pid1 < 0)
	{
		(exit_status = -1, perror(RED"Fork failed"WHITE));
		return ;
	}
	if (pid1 == 0)
	{
		exec_in_the_son(node);
		return ;
	}
	waitpid(pid1, &exit_status, 0);
}

void	exec_full_command(t_node *node, int fds[NUMBER_OF_FDS], t_exec_map exec_map[NUMBER_OF_EXEC_FUNCS])
{
	if (!node)
		return ;
	exec_map[node->type].function(node);
	if (node->type != SIMPLE_COMMAND)
	{
		if (!node->left)
			exec_full_command(node->right, fds, exec_map);
		else
			exec_full_command(node->left, fds, exec_map);
	}
    if (fds)
    {
        reset_standard_streams(fds);
    }
	unlink("here_doc.minishell");
}

void    reset_std(int saved_stdin, int saved_stdout)
{
    dup2(saved_stdin, STDIN_FILENO);
    dup2(saved_stdout, STDOUT_FILENO);
    close(saved_stdin);
    close(saved_stdout);
}

void    wait_all_pid(pid_t pid_tab[], int index)
{
    while (index >= 0) 
    {
        waitpid(pid_tab[index], &exit_status, WUNTRACED);
        index--;
    }
}

void    connect_pipe(int fd[2])
{
    close(fd[0]);
    dup2(fd[1], STDOUT_FILENO);
    close(fd[1]);
}

void    dup_tmp_fd(int tmp_fd)
{
    if (tmp_fd != STDIN_FILENO) 
    {
        dup2(tmp_fd, STDIN_FILENO);
        close(tmp_fd);
    }
}

void    error_message(char *message)
{
    perror(message);
    exit(EXIT_FAILURE);
}

void exec_pipes(t_node *node, t_exec_map exec_map[NUMBER_OF_EXEC_FUNCS]) 
{
    int     fd[2];
    int     tmp_fd;
    int     saved_stdin;
    int     saved_stdout;
    pid_t   pid_tab[node->number_of_pipes + 1];
    int     index;

    tmp_fd = dup(STDIN_FILENO);
    saved_stdin = dup(STDIN_FILENO);
    saved_stdout = dup(STDOUT_FILENO);
    index = 0;
    while (node->right) 
    {
        if (pipe(fd) == -1) 
            error_message("pipe");
        pid_tab[index] = fork();
        if (pid_tab[index] == -1) 
            error_message("fork");
        if (pid_tab[index] == 0) 
        {
            connect_pipe(fd);
            dup_tmp_fd(tmp_fd);
            exec_full_command(node->left, NULL, exec_map);
            clear_tree(node->head);
            exit(EXIT_SUCCESS);
        } 
        else 
        {
            close(fd[1]);
            if (tmp_fd != STDIN_FILENO) 
                close(tmp_fd);
            tmp_fd = fd[0];
			if (node->right)
            	node = node->right;
			else
                connect_pipe(fd);
        }
        index++;
    }
    dup_tmp_fd(tmp_fd);
    exec_full_command(node, NULL, exec_map);
    wait_all_pid(pid_tab, index - 1);
    reset_std(saved_stdin, saved_stdout);
}

void	execution(t_node *tree)
{
	t_exec_map	exec_map[NUMBER_OF_EXEC_FUNCS];
	int fds[NUMBER_OF_FDS];

	init_fds(fds);
	init_exec_func_map(exec_map);
	if (tree->number_of_pipes > 0)
		exec_pipes(tree, exec_map);
	else
		exec_full_command(tree, fds, exec_map);
	close_fds(fds);
}
