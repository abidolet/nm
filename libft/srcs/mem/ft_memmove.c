/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abidolet <abidolet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 00:00:06 by abidolet          #+#    #+#             */
/*   Updated: 2024/11/22 00:00:06 by abidolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

void	*ft_memmove(void *dest, const void *src, unsigned long n)
{
	void	*res;

	if (!dest && !src)
		return (0x0);
	res = dest;
	if (dest < src || (unsigned char *)dest >= (unsigned char *)src + n)
		while (n--)
			*(unsigned char *)dest++ = *(unsigned char *)src++;
	else
		while (n--)
			*((unsigned char *)dest + n) = *((unsigned char *)src + n);
	return (res);
}
