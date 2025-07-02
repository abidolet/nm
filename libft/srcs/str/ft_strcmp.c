/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abidolet <abidolet@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 16:38:19 by abidolet          #+#    #+#             */
/*   Updated: 2025/07/02 16:39:30 by abidolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

int	ft_strcmp(const char *s1, const char *s2)
{
	while ((*s1 || *s2) && *s1++ == *s2++)
		;
	return ((unsigned char)*(s1 - 1) - (unsigned char)*(s2 - 1));
}
