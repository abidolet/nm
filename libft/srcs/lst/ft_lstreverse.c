/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstreverse.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abidolet <abidolet@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 23:40:52 by abidolet          #+#    #+#             */
/*   Updated: 2025/07/04 00:08:40 by abidolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/lst.h"
#include <stddef.h>

void	ft_lstreverse(t_list **lst)
{
	t_list	*prev;
	t_list	*current;
	t_list	*next;

	if (lst && *lst)
	{
		prev = NULL;
		current = *lst;
		while (current != NULL)
		{
			next = current->next;
			current->next = prev;
			prev = current;
			current = next;
		}
		*lst = prev;
	}
}
