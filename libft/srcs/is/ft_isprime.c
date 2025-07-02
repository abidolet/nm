/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isprime.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abidolet <abidolet@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 00:35:27 by abidolet          #+#    #+#             */
/*   Updated: 2025/05/11 10:00:41 by abidolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include "libft/math.h"

bool	ft_isprime(size_t n)
{
	const size_t	max_divisor = ft_sqrt(n) + 1;
	size_t			i;

	if (n % 2 == 0 || n < 2)
		return (n == 2);
	i = 3;
	while (i <= max_divisor)
	{
		if (n % i == 0)
			return (0);
		i += 2;
	}
	return (1);
}
