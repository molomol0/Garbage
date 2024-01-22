/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environnement_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdenis <jdenis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 18:39:37 by jdenis            #+#    #+#             */
/*   Updated: 2024/01/22 13:05:09 by jdenis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "environnement.h"
#include "builtins.h"

void	sort(char **exports)
{
	size_t	index;
	size_t	index2;
	char	*tmp;

	index = 0;
	index2 = 0;
	while (exports[index])
	{
		index2 = index + 1;
		while (exports[index2])
		{
			if (ft_strcmp(exports[index], exports[index2]) > 0)
			{
				tmp = exports[index];
				exports[index] = exports[index2];
				exports[index2] = tmp;
			}
			index2++;
		}
		index++;
	}
}

void	put_last_quotes(char *new, size_t index2, bool equal)
{
	if (equal)
	{
		new[index2] = '"';
		(index2)++;
	}
	new[index2] = '\0';
}

char	*ft_strdup_with_quotes(char *str)
{
	char	*new;
	size_t	index;
	size_t	index2;
	bool	equal;

	index = 0;
	index2 = 0;
	equal = false;
	new = malloc(ft_strlen(str) + 3);
	if (!new)
		return (NULL);
	while (str[index])
	{
		new[index2] = str[index];
		if (str[index] == '=' && !equal)
		{
			equal = true;
			++(index2);
			new[index2] = '"';
		}
		index++;
		index2++;
	}
	put_last_quotes(new, index2, equal);
	return (new);
	// {
	// 	new[index2] = '"';
	// 	(index2)++;
	// }
	// new[index2] = '\0';
	// return (new);
}

// char	*ft_strdup_with_quotes(char *str)
// {
// 	char	*new;
// 	size_t	index;
// 	size_t	index2;
// 	size_t	index3;
// 	bool	equal;

// 	index = 0;
// 	index2 = 0;
// 	index3 = 0;
// 	equal = false;
// 	new = malloc(ft_strlen(str) + 3);
// 	if (!new)
// 		return (NULL);
// 	while (str[index])
// 	{
// 		new[index2] = str[index];
// 		if (str[index] == '=' && !equal)
// 			first_quotes(new, &index2, &index3, &equal);
// 		index++;
// 		index2++;
// 	}
// 	last_quotes(new, &index2, &index3);
// 	return (new);
// }

void	redo_export(t_envs *envs)
{
	size_t	index;
	int		fd;
	char	*path;

	index = 0;
	path = getenv("HOME");
	if (!path)
		return (error_message("Cannot find home path"));
	path = ft_strjoin(path, "/.temporary_export_minishell");
	fd = open(path, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
		return (error_message("Cannot open temporary export file"));
	while (envs->exports[index])
	{
		write(fd, envs->exports[index], ft_strlen(envs->exports[index]));
		write(fd, "\n", 1);
		index++;
	}
	free(path);
	close(fd);
}

void	redo_env(t_envs *envs)
{
	size_t	index;
	int		fd;
	char	*path;

	index = 0;
	path = getenv("HOME");
	if (!path)
		return (error_message("Cannot find home path"));
	path = ft_strjoin(path, "/.temporary_env_minishell");
	fd = open(path, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
		return (error_message("Cannot open temporary env file"));
	while (envs->env[index])
	{
		write(fd, envs->env[index], ft_strlen(envs->env[index]));
		write(fd, "\n", 1);
		index++;
	}
	free(path);
	close(fd);
}

void	redo_envs(t_envs *envs)
{
	redo_env(envs);
	redo_export(envs);
}
