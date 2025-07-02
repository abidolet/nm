/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abidolet <abidolet@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 13:23:44 by abidolet          #+#    #+#             */
/*   Updated: 2025/07/02 15:48:43 by abidolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdbool.h>
# include <stddef.h>

typedef struct s_flags
{
	bool	minus;
	bool	zero;
	bool	dot;
	size_t	width;
	bool	hash;
	bool	space;
	bool	plus;
	size_t	precision;
}	t_flags;

int	ft_printf(const char *str, ...);
int	ft_dprintf(int fd, const char *str, ...);
int	ft_putchar_fd(char c, int fd);
int	ft_putstr_fd(char *s, int fd);
int	ft_putnbr_fd(long n, int fd);
int	ft_puthex_fd(unsigned long n, char c, int fd);
int	ft_putptr_fd(void *ptr, int fd);

#endif
