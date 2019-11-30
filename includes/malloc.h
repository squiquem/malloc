/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: squiquem <squiquem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/20 22:55:18 by squiquem          #+#    #+#             */
/*   Updated: 2019/11/26 15:15:35 by squiquem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_H
# define MALLOC_H

# include <errno.h>
# include <sys/mman.h>
# include <unistd.h>
# include <pthread.h>
# include <stdbool.h>
# include <pthread.h>
# include "../libft/libft.h"

# define TINY_COEF      8
# define SMALL_COEF     112

# define TINY_LIM       128
# define SMALL_LIM      4096

extern pthread_mutex_t	g_mutex;

enum {
	TINY,
	SMALL,
	LARGE
};

typedef struct			s_block
{
	bool				free;
	size_t				size;
	void				*ptr;
	struct s_block		*prev;
	struct s_block		*next;
}						t_block;

typedef struct			s_page
{
	int					format;
	size_t				size;
	t_block				*start;
	struct s_page		*prev;
	struct s_page		*next;
}						t_page;

struct s_page			*g_page;

void					*malloc(size_t size);
void					*calloc(size_t count, size_t size);
void					*valloc(size_t size);
void					*aligned_alloc(size_t alignment, size_t size);
void					free(void *ptr);
void					*realloc(void *ptr, size_t size);
void					*reallocf(void *ptr, size_t size);
void					show_alloc_mem(void);

size_t					align(size_t alignment, size_t size);
size_t					align_page(int format, size_t size);
int						get_format(size_t size);

void					connect_blocks(t_block *prev, t_block *block,
						t_block *next);
void					set_block(t_block *block, size_t size, bool free);
void					split_block(t_block *block, size_t size);

void					*new_page(int format, size_t p_size);
void					delete_page(t_page *p);
t_page					*first_page(t_page *new, bool force);

t_block					*find_block(void *ptr, t_page **p);
void					*free_block(int format, size_t size);

void					attach_to_next(t_block *block);

void					*ft_error(void *ptr);

#endif
