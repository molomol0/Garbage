/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environnement_utils_4.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdenis <jdenis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 15:28:06 by jdenis            #+#    #+#             */
/*   Updated: 2024/01/16 15:28:52 by jdenis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "environnement.h"

void	last_quotes(char *new, size_t *index2, size_t *index3)
{
	if (new[*index3] == '"')
	{
		new[*index2] = '"';
		(*index2)++;
	}
	new[*index2] = '\0';
}

void	first_quotes(char *new, size_t *index2, size_t *index3, bool *equal)
{
	*equal = true;
	++(*index2);
	new[*index2] = '"';
	*index3 = *index2;
}

void	error_message(char *str)
{
	perror(str);
	return ;
}
