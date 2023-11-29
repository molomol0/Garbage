/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdenis <jdenis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/09 21:53:01 by dlacuey           #+#    #+#             */
/*   Updated: 2023/11/29 01:34:03 by jdenis           ###   ########.fr       */
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
	reset_standard_streams(fds);
	unlink("here_doc.minishell");
}

void printTree(t_node *node, int depth) {
    if (node != NULL) {
        // Afficher l'indentation en fonction de la profondeur du nœud
        for (int i = 0; i < depth; i++) {
            printf("  ");
        }

        // Afficher le type du nœud
        switch (node->type) {
            case SIMPLE_COMMAND:
                printf("Simple Command\n");
                break;
            case COMMAND_O_REDIRECT:
                printf("Command Output Redirect\n");
                break;
            case COMMAND_I_REDIRECT:
                printf("Command Input Redirect\n");
                break;
            case APPEND_REDIRECT:
                printf("Append Redirect\n");
                break;
            case HERE_DOCUMENT:
                printf("Here Document\n");
                break;
            case COMMAND_PIPE:
                printf("Command Pipe\n");
                break;
            default:
                printf("Unknown Type\n");
                break;
        }

        // Afficher les valeurs du vecteur de chaînes de caractères
        for (size_t i = 0; i < node->vector_strs.size; i++) {
            for (int j = 0; j <= depth; j++) {
                printf("  ");
            }
            printf("- %s\n", node->vector_strs.values[i]);
        }

        // Appeler la fonction récursivement pour le sous-arbre gauche et droit
        printTree(node->left, depth + 1);
        printTree(node->right, depth + 1);
    }
}

// Fonction d'affichage de l'arbre binaire à partir d'un nœud
void displayTree(t_node *node) {
    if (node != NULL) {
        printTree(node, 0);
    } else {
        printf("L'arbre est vide.\n");
    }
}


void exec_pipes(t_node *node, t_exec_map exec_map[NUMBER_OF_EXEC_FUNCS]) 
{
    int fd[2];
    int tmp_fd = dup(STDIN_FILENO);
	//displayTree(node);

    while (node->right) 
    {
        if (pipe(fd) == -1) 
        {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
        pid_t pid = fork();
        if (pid == -1) 
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        if (pid == 0) 
        {
            close(fd[0]);
            dup2(fd[1], STDOUT_FILENO);
            close(fd[1]);
            if (tmp_fd != STDIN_FILENO) 
            {
                dup2(tmp_fd, STDIN_FILENO);
                close(tmp_fd);
            }
            exec_full_command(node->left, NULL, exec_map);
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
			{
				close(fd[0]);
				dup2(fd[1], STDOUT_FILENO);
				close(fd[1]);
			}
        }
    }
    if (tmp_fd != STDIN_FILENO) 
    {
        dup2(tmp_fd, STDIN_FILENO);
        close(tmp_fd);
    }
	// dup2(fd[1], STDOUT_FILENO);
	// close(fd[1]);
	//printf("avant exec full command last\n");
	//displayTree(node);
    exec_full_command(node, NULL, exec_map);
	//printf("apres exec full command last\n");
    while (wait(NULL) != -1) {}
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
