/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstremove_if.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abidolet <abidolet@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 00:02:47 by abidolet          #+#    #+#             */
/*   Updated: 2025/07/04 00:18:35 by abidolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/lst.h"
#include <stddef.h>
#include <stdlib.h>

void	ft_lstremove_if(t_list **lst, void *data_ref, int (*cmp)(),
	void (*free_fct)(void *))
{
	t_list	*current;
	t_list	*prev;

	current = *lst;
	prev = NULL;
	while (current != NULL)
	{
		if (cmp(current->content, data_ref) == 0)
		{
			if (!prev)
				*lst = current->next;
			else
				prev->next = current->next;
			free_fct(current->content);
			free(current);
			if (!prev)
				current = *lst;
			else
				current = prev->next;
		}
		else
		{
			prev = current;
			current = current->next;
		}
	}
}
