/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abidolet <abidolet@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 21:27:34 by abidolet          #+#    #+#             */
/*   Updated: 2025/02/15 21:27:34 by abidolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

char	*ft_strnstr(const char *haystack, const char *needle, unsigned long len)
{
	unsigned long	i;
	unsigned long	j;

	if (!haystack || !needle)
		return ((0x0));
	else if (!*needle)
		return ((char *)haystack);
	i = -1;
	while (++i < len && haystack[i])
	{
		j = 0;
		while (i + j < len && haystack[i + j] == needle[j] && needle[j])
			j++;
		if (!needle[j])
			return ((char *)(haystack + i));
	}
	return ((0x0));
}
