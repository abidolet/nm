/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abidolet <abidolet@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 20:18:41 by abidolet          #+#    #+#             */
/*   Updated: 2025/07/05 01:24:01 by abidolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

void	*ft_memchr(const void *s, int c, unsigned long n)
{
	while (n--)
		if (*(unsigned char *)(s++) == (unsigned char)c)
			return ((void *)(s - 1));
	return ((0x0));
}
