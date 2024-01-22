/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environnement.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdenis <jdenis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 11:06:07 by jdenis            #+#    #+#             */
/*   Updated: 2024/01/16 15:30:55 by jdenis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENVIRONNEMENT_H
# define ENVIRONNEMENT_H

# include "colors.h"
# include "get_next_line.h"
# include "libft.h"
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>

typedef struct s_envs
{
	char	**env;
	char	**exports;
}			t_envs;

t_envs		*copy_env_and_export(void);
void		free_envs(t_envs *envs);
void		redo_envs(t_envs *envs);
void		update_envs(t_envs *envs);
char		*ft_strdup_with_quotes(char *str);
void		sort(char **exports);
void		redo_export(t_envs *envs);
void		redo_env(t_envs *envs);
void		redo_envs(t_envs *envs);
int			size_of_new_env(int fd);
char		**copy_environnement_from_file(int fd);
char		**copy_export_from_file(int fd);
void		update_export(t_envs *envs);
void		update_env(t_envs *envs);
char		*increased_shlvl(char *str);
char		**copy_environnement(void);
void		free_envs(t_envs *envs);
void		update_envs(t_envs *envs);
void		last_quotes(char *new, size_t *index2, size_t *index3);
void		first_quotes(char *new, size_t *index2,
				size_t *index3, bool *equal);
void		error_message(char *str);

#endif
