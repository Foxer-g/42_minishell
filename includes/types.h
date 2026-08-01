/* ************************************************************************** */
/*                                                                            */
/*                                                       ⠀⠀⠀⠀⠀⠀⣴⣾⣿⣿⣿⠷⢠⣤⡀      */
/*   types.h                                             ⠀⢀⣀⣀⣛⡑⢶⣬⣭⢩⣶⣿⣷⣭⢻⣦⡀    */
/*                                                       ⣴⠛⢿⡟⠛⢿⣦⠹⣿⡆⣿⣿⣿⣿⣷⢩⡶⠃   */
/*   By: neumann </var/spool/mail/neumann>               ⣿⣖⠾⢗⣶⣾⣿⡇⠿⠷⠸⠿⢟⣛⡵⣫     */
/*                                                       ⠙⢿⣿⣿⣿⣿⣿⣿⣮⣭⣭⣭⡭⣶⣾⣿     */
/*   Created: 2026/07/28 19:34:27 by neumann            ⠀⠀⣿⣿⣿⠛⠛⠛⣿⣿⣿⠁⠀⠀⠉⠁      */
/*   Updated: 2026/07/28 19:51:58 by neumann            ⠀⠀⠙⠛⠉⠀⠀⠀⠻⠿⠟           */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPES_H
# define TYPES_H
# include <stdint.h>
# include <stdbool.h>

// @doc t_validator
// @kind type
// @desc Enum to index into the default validator functions in g_validators.
// @field DIGIT: uint8_t, Only digits.
// @field ALPHA: uint8_t, Only alphabetic characters.
// @field SPACE: uint8_t, Only space characters.
// @field PRINT: uint8_t, Only printable characters.
// @field ASCII: uint8_t, Only ascii characters.
// @field ALNUM: uint8_t, Only alphanumeric characters.
typedef enum e_validator
{
	DIGIT,
	ALPHA,
	SPACE,
	PRINT,
	ASCII,
	ALNUM
}	t_validator;

// @doc t_validator_fn
// @kind type
// @desc Type that describes a validating function for str_is_valid.
typedef bool	(*t_validator_fn)(int32_t val);

// @doc t_ptr
// @kind type
// @desc Type that allows for clean pointer to uintptr_t casts. Don't ask.
// @field ptr: void *, The pointer casted from/to.
// @field addr: uintptr_t, The uintptr_t representation of the pointer.
typedef union u_ptr
{
	void		*ptr;
	uintptr_t	addr;
}	t_ptr;

// @doc t_list
// @kind type
// @desc Linked list type.
// @field content: void *, The content of the node.
// @field next: t_list *, The next node.
typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}				t_list;

// @doc t_ffile
// @kind type
// @desc Fake FILE type, I was too lazy to finish my implemenation so this
// @desc is just a cheap alias for int. oh and it avoids mixing int
// @desc and int32_t I guess.
typedef int				t_ffile;

// @doc t_ffile
// @kind type
// @desc Yup, a whole trucking iovec, don't ask.
// @field iov_base: void *, `man iovec`, stop your suffering.
// @field iov_len: uintmax_t, Have you read the doc for iov_base?
typedef struct s_iovec
{
	void		*iov_base;
	uintmax_t	iov_len;
}	t_iovec;
#endif // TYPES_H
