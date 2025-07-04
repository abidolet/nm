/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abidolet <abidolet@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 23:37:34 by abidolet          #+#    #+#             */
/*   Updated: 2025/07/04 23:22:10 by abidolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/str.h"
#include "libft/math.h"
#include <stdlib.h>

char	*ft_strndup(const char *s, unsigned long n)
{
	char	*res;

	if (!s)
		return (NULL);
	n = ft_min(ft_strlen(s), n) + 1;
	res = malloc(n);
	if (!res)
		return (NULL);
	ft_strlcpy(res, s, n);
	return (res);
}
