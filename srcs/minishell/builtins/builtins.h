/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdenis <jdenis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 03:13:15 by jdenis            #+#    #+#             */
/*   Updated: 2024/01/16 18:35:24 by jdenis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include <stdbool.h>
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include "environnement.h"

int		ft_echo(char **str);
int		ft_pwd(t_envs *envs);
int		ft_cd(t_envs *envs, char *command);
int		ft_env(t_envs *envs);
int		ft_exit(t_envs *envs, char *status);
int		ft_export(t_envs *envs, char **input);
int		ft_unset(t_envs *envs, char **input);
void	ft_setenv(char *name, char *value, t_envs *envs);
int		exec_builtin(char **command, t_envs *envs);
bool	is_a_builtin(char *command);
char	*ft_getenv(char *name, t_envs *envs);
char	*ft_compact_strs(char **strs);
void	remove_environment_variable(char **envs, char *variable_name);
bool	ft_is_in_env(char *name, char **envs);
int		add_var_env(char ***envs, char **input);
char	*ft_get_name(char *input);
char	*ft_strcat(char *dest, char *src);
int		add_var_exp(char ***envs, char **input);
int		print_empty_export(t_envs *envs);
char	*ft_getenv(char *name, t_envs *envs);
void	change_pwd_in_env(char *path, t_envs *envs);

#endif