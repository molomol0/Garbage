/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdenis <jdenis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 08:41:00 by jdenis            #+#    #+#             */
/*   Updated: 2024/01/16 21:20:26 by jdenis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "environnement.h"
#include "libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// extern char **environ;

int	add_variables(t_envs *envs, char **input)
{
	char	*name;

	name = ft_get_name(input[1]);
	if (ft_is_in_env(name, envs->env) == true)
		remove_environment_variable(envs->env, name);
	if (ft_is_in_env(name, envs->exports) == true)
		remove_environment_variable(envs->exports, name);
	if (add_var_env(&(envs->env), input) == EXIT_FAILURE
		|| add_var_exp(&(envs->exports), input) == EXIT_FAILURE)
	{
		free(name);
		return (EXIT_FAILURE);
	}
	free(name);
	redo_envs(envs);
	return (EXIT_SUCCESS);
}

bool	is_alpha_name(char *name)
{
	size_t	index;

	index = 0;
	while (name[index])
	{
		if (!ft_isalpha(name[index]))
			return (false);
		index++;
	}
	return (true);
}

int	ft_export(t_envs *envs, char **input)
{
	char	*name;

	if (!input[1])
	{
		return (print_empty_export(envs));
	}
	else
	{
		name = ft_get_name(input[1]);
		if (input[1][0] == '=' || !is_alpha_name(name))
		{
			printf("minishell: export: `%s': not a valid identifier\n",
				input[1]);
			free(name);
			return (EXIT_FAILURE);
		}
		free(name);
	}
	return (add_variables(envs, input));
}
