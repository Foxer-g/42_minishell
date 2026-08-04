/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                             ⠀⢀⣀⣀⣛⡑⢶⣬⣭⢩⣶⣿⣷⣭⢻⣦⡀    */
/*                                                    +:+ +:+         +:+     */
/*   By: rboutelo <rboutelo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 14:45:16 by rboutelo          #+#    #+#             */
/*   Updated: 2026/08/04 17:40:45 by neumann            ⠀⠀⠙⠛⠉⠀⠀⠀⠻⠿⠟           */
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
# include <stdarg.h>
# ifndef FORBIDDEN_ERRNO_H
#  include <errno.h>
# endif //FORBIDDEN_ERRNO_H
# include "types.h"
# include "file.h"
# include "dynamic_list.h"
# define DEC_BASE "0123456789"
# define HEX_LOW "0123456789abcdef"
# define HEX_UPP "0123456789ABCDEF"

/* ******************** */
/*        MEMORY        */
/* ******************** */

/* **************** */
/*    MANAGEMENT    */
/* **************** */
void		*ft_calloc(size_t nmemb, size_t size);
void		*ft_realloc(void *ptr, size_t size);
void		*ft_recalloc(void *ptr, uintmax_t nmemb, uintmax_t size);
void		*ft_free_nt_tab(char **str, int32_t alloc_count);

/* **************** */
/*    ALTERATION    */
/* **************** */
void		ft_bzero(void *s, size_t n);
void		*ft_memset(void *s, int32_t c, size_t n);
void		*ft_memcpy(void *dest, const void *src, size_t n);
void		*ft_memmove(void *dest, const void *src, size_t n);

/* **************** */
/*    COMPARISON    */
/* **************** */
void		*ft_memchr(const void *s, int32_t c, size_t n);
int32_t		ft_memcmp(const void *s1, const void *s2, size_t n);

/* ******************** */
/*        PRINT         */
/* ******************** */
int32_t		ft_printf(const char *format, ...);
int32_t		ft_dprintf(t_ffile fd, const char *format, ...);

int32_t		ft_putchar_fd(char c, t_ffile fd);
int32_t		ft_putstr_fd(const char *s, t_ffile fd);
int32_t		ft_putstrn_fd(const char *s, t_ffile fd, int32_t len);
int32_t		ft_putstr_fd_null(const char *s, int32_t fd);
int32_t		ft_putendl_fd(const char *s, t_ffile fd);
int32_t		ft_putendl_fd_null(const char *s, int32_t fd);

int32_t		ft_putnbr_fd(int32_t n, t_ffile fd);
int32_t		ft_putuint_base_fd(uint64_t n, char *base, t_ffile fd, bool *error);

/* ******************** */
/*        STRING        */
/* ******************** */

/* **************** */
/*    ALTERATION    */
/* **************** */
void		ft_striteri(char *s, void (*f)(uint32_t, char*));
int32_t		ft_toupper(int32_t c);
int32_t		ft_tolower(int32_t c);
size_t		ft_strlcpy(char *dst, const char *src, size_t dsize);
size_t		ft_strlcat(char *dst, const char *src, size_t dsize);

/* **************** */
/*      CHECKS      */
/* **************** */
bool		ft_isalpha(int32_t c);
bool		ft_isdigit(int32_t c);
bool		ft_isalnum(int32_t c);
bool		ft_isascii(int32_t c);
bool		ft_isprint(int32_t c);
bool		ft_isspace(int32_t c);

bool		ft_str_is_valid(char *str, t_validator_fn validator);

int32_t		ft_strcmp(const char *s1, const char *s2);
int32_t		ft_strncmp(const char *s1, const char *s2, int32_t n);
char		*ft_strnstr(const char *big, const char *little, size_t len);

/* **************** */
/*    CONVERSION    */
/* **************** */
int32_t		ft_atoi(const char *str);
char		*ft_itoa(int32_t n);

/* **************** */
/*   MANIPULATION   */
/* **************** */
uintmax_t	ft_strlen(const char *str);
uintmax_t	ft_strlen_until(const char *str, char c);
uint64_t	ft_fwlen(const char *str);
char		*ft_strchr(const char *s, int32_t c);
char		*ft_strrchr(const char *s, int32_t c);

/* **************** */
/*        NEW       */
/* **************** */
char		*ft_substr(char const *s, uint32_t start, size_t len);
char		*ft_strmapi(char const *s, char (*f)(uint32_t, char));
char		*ft_strdup(const char *s);
char		*ft_strndup(const char *s, uint64_t n);

char		**ft_split(char const *s, char c);
char		**ft_preserving_split(char const *s, char c);

char		*ft_strjoin(char const *s1, char const *s2);
char		*ft_extend(char *to_extend, char *extender);

char		*ft_strtrim(char const *s1, char const *set);

/* ******************** */
/*        UTILS         */
/* ******************** */

/* **************** */
/*        ENV       */
/* **************** */
char		*ft_get_env(const char *name, char *const *env);
void		ft_set_var(char **var, char *val);
void		ft_set_env(const char *name, char ***env, char *value);
void		ft_set_exit_code(int32_t code, char ***env);
char		**ft_copy_env(const char **env);

/* **************** */
/*    EXECUTION     */
/* **************** */
char		*ft_get_executable(const char *str);
char		*ft_find_exec(const char *name, char const **env);

/* **************** */
/*       MATH       */
/* **************** */
uintmax_t	ft_min(uintmax_t a, uintmax_t b);
uintmax_t	ft_max(uintmax_t a, uintmax_t b);
uintmax_t	ft_nt_tablen(void **tab);

/* ******************** */
/*         LIST         */
/* ******************** */

/* **************** */
/*      LINKED      */
/* **************** */
t_list		*ft_lstnew(void *content);
void		ft_lstadd_front(t_list **lst, t_list *new);
int32_t		ft_lstsize(t_list *lst);
t_list		*ft_lstlast(t_list *lst);
void		ft_lstadd_back(t_list **lst, t_list *new);
void		ft_lstdelone(t_list *lst, void (*del)(void *));
void		ft_lstclear(t_list **lst, void (*del)(void *));
void		ft_lstiter(t_list *lst, void (*f)(void *));
t_list		*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));

/* ********************************** */
/*             INTERNALS              */
/* ********************************** */
bool		in_set(char c);
int32_t		get_next(const char *format, bool *error);
int32_t		print_pointer(void *pointer, t_ffile fd, bool *error);
int32_t		inject(const char *format, va_list args, bool *error, t_ffile fd);
int32_t		pre_check(t_ffile fd, const char *format, va_list args);
#endif
