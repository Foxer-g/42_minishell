/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                                    _             */
/*                                                    +:+ +:+         +:+     */
/*   By: rboutelo <rboutelo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 14:45:16 by rboutelo          #+#    #+#             */
/*   Updated: 2026/05/04 05:05:49 by neumann                                  */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H
# if BUFFER_SIZE >= SIZE_MAX
#  undef BUFFER_SIZE
# endif
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1024
# endif //BUFFER_SIZE
# include <unistd.h>
# include <stdlib.h>
# include <stdbool.h>
# include <stdint.h>
# include <stdarg.h>
# include "file.h"
# include "dynamic_list.h"
# define DEC_BASE "0123456789"
# define HEX_LOW "0123456789abcdef"
# define HEX_UPP "0123456789ABCDEF"
# undef SPACE

bool		ft_isalpha(int32_t c);
bool		ft_isdigit(int32_t c);
bool		ft_isalnum(int32_t c);
bool		ft_isascii(int32_t c);
bool		ft_isprint(int32_t c);
bool		ft_isspace(int32_t c);
uintmax_t	ft_strlen(const char *str);
uintmax_t	ft_strlen_until(const char *str, char c);
char		*extend(char *to_extend, char *extender);
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
void		*ft_realloc(void *ptr, size_t size);
char		*ft_strdup(const char *s);
char		*ft_strndup(const char *s, uint64_t n);
char		*ft_substr(char const *s, uint32_t start, size_t len);
char		*ft_strjoin(char const *s1, char const *s2);
char		*ft_strtrim(char const *s1, char const *set);
char		**ft_split(char const *s, char c);
char		*ft_itoa(int32_t n);
char		*ft_strmapi(char const *s, char (*f)(uint32_t, char));
void		ft_striteri(char *s, void (*f)(uint32_t, char*));
int32_t		ft_putchar_fd(char c, t_ffile fd);
int32_t		ft_putstr_fd(const char *s, t_ffile fd);
int32_t		ft_putstrn_fd(const char *s, t_ffile fd, int32_t len);
int32_t		ft_putstr_fd_null(const char *s, int32_t fd);
int32_t		ft_putendl_fd(const char *s, t_ffile fd);
int32_t		ft_putendl_fd_null(const char *s, int32_t fd);
int32_t		ft_putnbr_fd(int32_t n, t_ffile fd);
int32_t		ft_putuint_base_fd(uint64_t n, char *base, t_ffile fd, bool *error);
int32_t		ft_dprintf(t_ffile fd, const char *format, ...);
int32_t		ft_printf(const char *format, ...);
int32_t		ft_strcmp(const char *s1, const char *s2);
char		*get_env(const char *name, char *const *env);
char		*get_executable(const char *str);
char		*find_exec(const char *name, char const **env);
char		*get_next_line(int fd);
uintmax_t	min(uintmax_t a, uintmax_t b);
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
void		ft_lstadd_front(t_list **lst, t_list *new);
int32_t		ft_lstsize(t_list *lst);
t_list		*ft_lstlast(t_list *lst);
void		ft_lstadd_back(t_list **lst, t_list *new);
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

typedef bool	(*t_validator_fn)(int32_t val);

/* ********************************** */
/*             INTERNALS              */
/* ********************************** */
bool		in_set(char c);
int32_t		get_next(const char *format, bool *error);
int32_t		print_pointer(void *pointer, t_ffile fd, bool *error);
int32_t		inject(const char *format, va_list args, bool *error, t_ffile fd);
int32_t		pre_check(t_ffile fd, const char *format, va_list args);
#endif
