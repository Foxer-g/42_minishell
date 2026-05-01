/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rboutelo <rboutelo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 14:45:16 by rboutelo          #+#    #+#             */
/*   Updated: 2026/03/10 01:41:28 by rboutelo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "file.h"

bool		ft_isalpha(int32_t c);
bool		ft_isdigit(int32_t c);
bool		ft_isalnum(int32_t c);
bool		ft_isascii(int32_t c);
bool		ft_isprint(int32_t c);
bool		ft_isspace(int32_t c);
uintmax_t	ft_strlen(const char *str);
uint64_t	ft_fwlen(const char *str);
void		*ft_memset(void *s, int32_t c, size_t n);
void		ft_bzero(void *s, size_t n);
void		*ft_memcpy(void *dest, const void *src, size_t n);
void		*ft_memmove(void *dest, const void *src, size_t n);
size_t		ft_strlcpy(char *dst, const char *src, size_t dsize);
size_t		ft_strlcat(char *dst, const char *src, size_t dsize);
int32_t		ft_toupper(int32_t c);
int32_t		ft_tolower(int32_t c);
char		*ft_strchr(const char *s, int32_t c);
char		*ft_strrchr(const char *s, int32_t c);
int32_t		ft_strncmp(const char *s1, const char *s2, int32_t n);
void		*ft_memchr(const void *s, int32_t c, size_t n);
int32_t		ft_memcmp(const void *s1, const void *s2, size_t n);
char		*ft_strnstr(const char *big, const char *little, size_t len);
int32_t		ft_atoi(const char *str);
void		*ft_calloc(size_t nmemb, size_t size);
char		*ft_strdup(const char *s);
char		*ft_strndup(const char *s, uint64_t n);
char		*ft_substr(char const *s, uint32_t start, size_t len);
char		*ft_strjoin(char const *s1, char const *s2);
char		*ft_strtrim(char const *s1, char const *set);
char		**ft_split(char const *s, char c);
char		*ft_itoa(int32_t n);
char		*ft_strmapi(char const *s, char (*f)(uint32_t, char));
void		ft_striteri(char *s, void (*f)(uint32_t, char*));
void		ft_putchar_fd(char c, t_ffile fd);
void		ft_putstr_fd(char *s, t_ffile fd);
void		ft_putendl_fd(char *s, t_ffile fd);
void		ft_putnbr_fd(int32_t n, t_ffile fd);
int32_t		ft_strcmp(const char *s1, const char *s2);
char		*get_env(const char *name, char *const *env);
char		*get_executable(const char *str);
char		*find_exec(const char *name, char const **env);
void		*free_nt_tab(char **str, int32_t alloc_count);
int32_t		nt_tablen(void **tab);
typedef union u_ptr
{
	void		*ptr;
	uintptr_t	addr;
}	t_ptr;
typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}				t_list;
t_list		*ft_lstnew(void *content);
void		ft_lstadd_front(t_list **lst, t_list *new_elm);
int32_t		ft_lstsize(t_list *lst);
t_list		*ft_lstlast(t_list *lst);
void		ft_lstadd_back(t_list **lst, t_list *new_elm);
void		ft_lstdelone(t_list *lst, void (*del)(void *));
void		ft_lstclear(t_list **lst, void (*del)(void *));
void		ft_lstiter(t_list *lst, void (*f)(void *));
t_list		*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));

typedef enum e_validator
{
	DIGIT,
	ALPHA,
	SPACE,
	PRINT,
	ASCII,
	ALNUM
}	t_validator;

typedef bool	t_validator_fn(int32_t val);
