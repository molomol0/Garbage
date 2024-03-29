/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdenis <jdenis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/29 08:37:16 by dlacuey           #+#    #+#             */
/*   Updated: 2024/01/15 18:31:02 by jdenis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "colors.h"
#include "parser.h"
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

void	redirection_output(t_node *node)
{
	int	fd;

	fd = open(node->right->vector_strs.values[0], O_WRONLY | O_CREAT | O_TRUNC,
			0644);
	if (fd < 0)
		(perror(RED "Open ici failed" WHITE), exit(1));
	if (dup2(fd, STDOUT_FILENO) < 0)
		(perror(RED "Dup2 failed" WHITE), exit(1));
	if (close(fd) < 0)
		(perror(RED "Close failed" WHITE), exit(1));
}

void	append_output(t_node *node)
{
	int	fd;

	fd = open(node->right->vector_strs.values[0], O_WRONLY | O_CREAT | O_APPEND,
			0644);
	if (fd < 0)
		(perror(RED "Open failed" WHITE), exit(1));
	if (dup2(fd, STDOUT_FILENO) < 0)
		(perror(RED "Dup2 failed" WHITE), exit(1));
	if (close(fd) < 0)
		(perror(RED "Close failed" WHITE), exit(1));
}

void	redirection_input(t_node *node)
{
	int	fd;

	fd = open(node->right->vector_strs.values[0], O_RDONLY);
	if (fd < 0)
		(perror(RED "Open failed" WHITE), exit(1));
	if (dup2(fd, STDIN_FILENO) < 0)
		(perror(RED "Dup2 failed" WHITE), exit(1));
	if (close(fd) < 0)
		(perror(RED "Close failed" WHITE), exit(1));
}
