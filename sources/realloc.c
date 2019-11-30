/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: squiquem <squiquem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/22 00:45:31 by squiquem          #+#    #+#             */
/*   Updated: 2019/11/26 15:03:32 by squiquem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

/*
**	REALLOCATE
**	Realloc without changing data structures (block + pages)
*/

static void		*reallocate(t_block *block, size_t size)
{
	void	*new;

	if (!(new = malloc(size)))
		return (new);
	if (block)
	{
		ft_memcpy(new, block->ptr, block->size);
		free(block->ptr);
	}
	return (new);
}

/*
**	EXTEND_BLOCK
**	Merges with next block and splits the created one to extend size
**	2 blocks -> 2 blocks
*/

static void		*extend_block(t_block *block, size_t size)
{
	size_t	full_size;

	if (!block->next || !block->next->free || (block->size + block->next->size
				+ align(16, sizeof(t_block)) < size))
		return (NULL);
	full_size = block->size + block->next->size;
	attach_to_next(block);
	set_block(block, size, false);
	if (full_size > size)
		split_block(block, full_size - size);
	return (block->ptr);
}

/*
**	REDUCE_BLOCK
**	Splits one block to 2, to reduce size
**	1 block -> 2 blocks
*/

static t_block	*reduce_block(t_block *block, size_t size)
{
	size_t old_sz;

	old_sz = block->size;
	if (old_sz - size > align(16, sizeof(t_block)))
	{
		block->size = size;
		split_block(block, old_sz - size - align(16, sizeof(t_block)));
	}
	return (block->ptr);
}

/*
**	REALLOC
**	The realloc() function tries to change the size of the allocation pointed
**	to by ptr to size, and returns ptr.  If there is not enough room to enlarge
**	the memory allocation pointed to by ptr, realloc() creates a new allocation,
**	copies as much of the old data pointed to by ptr as will fit to the new
**	allocation, frees the old allocation, and returns a pointer to the allocated
**	memory.  If ptr is NULL, realloc() is identical to a call to malloc() for
**	size bytes. If size is zero and ptr is not NULL, a new, minimum sized object
**	is allocated and the original object is freed.  When extending a region
**	allocated with calloc(3), realloc(3) does not guarantee that the additional
**	memory is also zero-filled.
*/

void			*realloc(void *ptr, size_t size)
{
	t_block	*block;
	t_page	*p;

	size = align(16, size);
	if (!ptr || !size)
		return (reallocate(ptr, size));
	p = first_page(NULL, false);
	if (!(block = find_block(ptr, &p)))
		return (ft_error(block));
	if (block->size > size)
		ptr = reduce_block(block, size);
	else if (block->size != size && !(ptr = extend_block(block, size)))
		return (reallocate(block, size));
	return (ft_error(ptr));
}

/*
**	REALLOCF
**	The reallocf() function is identical to the realloc() function, except that
**	it will free the passed pointer when the requested memory cannot be
**	allocated.  This is a FreeBSD specific API designed to ease the problems
**	with traditional coding styles for realloc causing memory leaks in
**	libraries.
*/

void			*reallocf(void *ptr, size_t size)
{
	void *new_alloc;

	if (!(new_alloc = realloc(ptr, size)))
		free(ptr);
	return (new_alloc);
}
