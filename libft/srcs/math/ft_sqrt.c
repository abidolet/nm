/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sqrt.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abidolet <abidolet@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 00:19:01 by abidolet          #+#    #+#             */
/*   Updated: 2025/05/11 16:10:45 by abidolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/math.h"

double	ft_sqrt(double n)
{
	double			res;
	double			tmp;
	const double	epsilon = 1e-10;

	if (n < 0.0)
		return (NAN);
	else if (n == 0 || n == 1)
		return (n);
	res = n * 0.5;
	tmp = res + 1 + epsilon;
	while (ft_abs(tmp - res) > epsilon)
	{
		tmp = res;
		res = (res + n / res) * 0.5;
	}
	return (res);
}
