/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environnement_utils_2.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdenis <jdenis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 18:44:16 by jdenis            #+#    #+#             */
/*   Updated: 2024/01/22 13:05:45 by jdenis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "environnement.h"
#include "builtins.h"

int	size_of_new_env(int fd)
{
	char	*str;
	int		size;
	int		fd_copy;

	size = 0;
	fd_copy = fd;
	str = get_next_line(fd_copy);
	while (str)
	{
		size++;
		free(str);
		str = get_next_line(fd_copy);
	}
	free(str);
	return (size);
}

int	size_of_new_export(int fd)
{
	char	*str;
	int		size;
	int		fd_copy;

	size = 0;
	fd_copy = fd;
	str = get_next_line(fd_copy);
	while (str)
	{
		size++;
		free(str);
		str = get_next_line(fd_copy);
	}
	free(str);
	return (size);
}

char	*get_line(char *str)
{
	char	*tmp;

	tmp = ft_strdup(str);
	free(str);
	tmp[ft_strlen(tmp) - 1] = '\0';
	return (tmp);
}

char	**copy_environnement_from_file(int fd)
{
	size_t	index;
	size_t	length;
	char	**new;
	char	*str;

	length = 100;
	index = 0;
	new = malloc(sizeof(char *) * (length + 1));
	if (!new)
		return (NULL);
	str = get_next_line(fd);
	while (str)
	{
		new[index] = get_line(str);
		if (!new[index])
		{
			free_strs(new);
			return (NULL);
		}
		str = get_next_line(fd);
		index++;
	}
	free(str);
	new[index] = NULL;
	return (new);
}

char	**copy_export_exit(char **new)
{
	free_strs(new);
	return (NULL);
}

char	**copy_export_from_file(int fd)
{
	size_t	index;
	size_t	length;
	char	**new;
	char	*str;

	length = 100;
	index = 0;
	new = malloc(sizeof(char *) * (length + 1));
	if (!new)
		return (NULL);
	str = get_next_line(fd);
	while (str)
	{
		new[index] = get_line(str);
		if (!new[index])
			return (copy_export_exit(new));
		str = get_next_line(fd);
		index++;
	}
	free(str);
	new[index] = NULL;
	sort(new);
	return (new);
}

char	*env_path(void)
{
	char	*path;

	path = getenv("HOME");
	if (!path)
	{
		perror(RED "Cannot find home path" WHITE);
		return (NULL);
	}
	path = ft_strjoin(path, "/.temporary_env_minishell");
	return (path);
}

void	update_export(t_envs *envs)
{
	int		fd;
	char	*path;

	path = env_path();
	if (access(path, F_OK) != -1)
	{
		fd = open(path, O_RDONLY, 0644);
		if (fd < 0)
		{
			perror(RED "Cannot open temporary export file" WHITE);
			return ;
		}
		free_strs(envs->exports);
		envs->exports = copy_export_from_file(fd);
		close(fd);
		unlink(path);
	}
	free(path);
}

void	error_update_env(char *path)
{
	perror(RED "Cannot open temporary env file" WHITE);
	free(path);
	return ;
}

void	update_env(t_envs *envs)
{
	int		fd;
	char	*path;
	char	*pwd;

	path = env_path();
	if (!path)
		return ;
	if (access(path, F_OK) != -1)
	{
		fd = open(path, O_RDONLY, 0644);
		if (fd < 0)
			return (error_update_env(path));
		free_strs(envs->env);
		envs->env = copy_environnement_from_file(fd);
		close(fd);
		unlink(path);
		pwd = ft_getenv("PWD", envs);
		if (chdir(pwd) != 0)
			perror(RED "Cannot change directory" WHITE);
		free(pwd);
	}
	free(path);
}
