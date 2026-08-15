/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dynamic_list.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rboutelo <rboutelo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/17 05:35:19 by rboutelo          #+#    #+#             */
/*   Updated: 2026/08/15 06:15:05 by rboutelo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DYNAMIC_LIST_H
# define DYNAMIC_LIST_H
# include <stdint.h>
# include <stdlib.h>
# include "libft.h"
# define ARR_DEFAULT_SIZE 129

typedef enum e_type
{
	N8,
	N16,
	N32,
	N64,
	PTR,
	FLOAT,
	DOUBLE,
	LDOUBLE
}	t_type;

typedef struct s_list_header
{
	uintmax_t	count;
	uintmax_t	capacity;
	t_type		type;
}	t_header;

union u_value
{
	char		c;
	uint16_t	u16;
	uint32_t	u32;
	uint64_t	u64;
	void		*ptr;
	float		f;
	double		d;
	long double	ld;
};

typedef struct s_elem
{
	t_type			type;
	union u_value	value;
}	t_elem;

void	*ft_init_array(t_elem elem);
void	*ft_extend_array(void **arr);
void	ft_arr_push(void **arr, t_elem elem);
void	ft_arr_free(void **arr);

#endif //DYNAMIC_LIST_H
