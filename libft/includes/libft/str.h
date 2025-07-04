/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abidolet <abidolet@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 22:09:58 by abidolet          #+#    #+#             */
/*   Updated: 2025/07/04 23:29:32 by abidolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STR_H
# define STR_H

char			**ft_split(const char *s, char c);
void			*free_arr(void **arr);
char			*ft_strchr(const char *str, int c);
int				ft_strcmp(const char *s1, const char *s2);
char			*ft_strcpy(char *dest, const char *src);
char			*ft_strdup(const char *s);
void			ft_striteri(char *s, void (*f)(unsigned int, char*));
char			*ft_strjoin(char const *s1, char const *s2);
unsigned long	ft_strlcat(char *dest, const char *src, unsigned long size);
unsigned long	ft_strlcpy(char *dest, const char *src, unsigned long size);
unsigned long	ft_strlen(const char *str);
char			*ft_strmapi(char const *s, char (*f)(unsigned int, char));
int				ft_strncmp(const char *s1, const char *s2, unsigned long n);
char			*ft_strndup(const char *s, unsigned long n);
char			*ft_strnstr(const char *haystack, const char *needle,
					unsigned long len);
char			*ft_strrchr(const char *str, int c);
char			*ft_strtrim(char const *s1, char const *set);
char			*ft_substr(char const *s, unsigned int start,
					unsigned long len);

#endif
