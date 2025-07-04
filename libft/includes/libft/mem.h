/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mem.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abidolet <abidolet@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 22:15:17 by abidolet          #+#    #+#             */
/*   Updated: 2025/07/04 23:29:30 by abidolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MEM_H
# define MEM_H

void	ft_bzero(void *s, unsigned long n);
void	*ft_calloc(unsigned long num, unsigned long size);
void	*ft_memchr(const void *s, int c, unsigned long n);
int		ft_memcmp(const void *s1, const void *s2, unsigned long n);
void	*ft_memcpy(void *dest, const void *src, unsigned long n);
void	*ft_memmove(void *dest, const void *src, unsigned long n);
void	*ft_memset(void *s, int c, unsigned long n);
void	*realloc(void *ptr, unsigned long size);

#endif
