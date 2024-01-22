/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlacuey <dlacuey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 06:53:23 by dlacuey           #+#    #+#             */
/*   Updated: 2024/01/16 06:54:16 by dlacuey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdbool.h>
#include <stdio.h>

static void	remove_char(char *value)
{
	while (*value)
	{
		*value = *(value + 1);
		value++;
	}
}

static void	remove_untile_quote(char **value)
{
	remove_char(*value);
	while (**value && **value != '\'')
		(*value)++;
	remove_char(*value);
	(*value)--;
}

static void	remove_untile_double_quote(char **value)
{
	remove_char(*value);
	while (**value && **value != '\"')
		(*value)++;
	remove_char(*value);
	(*value)--;
}

static void	remove_quotes_from_value(char *value)
{
	while (*value)
	{
		if (*value == '\'')
			remove_untile_quote(&value);
		if (*value == '\"')
			remove_untile_double_quote(&value);
		value++;
	}
}

void	remove_quotes(char **values)
{
	int	index;

	index = 0;
	if (values == NULL)
		return ;
	while (values[index])
	{
		remove_quotes_from_value(values[index]);
		index++;
	}
}
