/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: squiquem <squiquem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 23:44:30 by squiquem          #+#    #+#             */
/*   Updated: 2019/11/26 15:02:05 by squiquem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

/*
**	FIND_BLOCK_IN_PAGE
**	Returns block ptr if block is in page, else NULL
*/

static t_block	*find_block_in_page(t_page *p, void *ptr)
{
	t_block	*block;

	block = p->start;
	while (block)
	{
		if (block->ptr == ptr && !block->free)
			return (block);
		block = block->next;
	}
	return (NULL);
}

/*
**	FIND_BLOCK
**	Returns block ptr if block is created by our malloc, else NULL
*/

t_block			*find_block(void *ptr, t_page **p)
{
	t_block	*search;

	search = NULL;
	while (*p)
	{
		if (ptr > (void*)*p && (ptr < ((void*)*p + (*p)->size))
				&& (search = find_block_in_page(*p, ptr)))
			break ;
		*p = (*p)->next;
	}
	return (search);
}

/*
**	FREE_BLOCK_IN_PAGE
**	Looks for free & big enough block in page p
**	Returns the block ptr or NULL
*/

static t_block	*free_block_in_page(t_page *p, size_t size)
{
	t_block	*search;

	search = p->start;
	while (search && (!search->free || search->size < size))
		search = search->next;
	return (search);
}

/*
**	FREE_BLOCK
**	Looks for free block in structure with good size
**	Returns the ptr and split in blocks or creates an empty page
*/

void			*free_block(int format, size_t size)
{
	t_page	*p;
	t_block	*search;
	size_t	old_sz;

	p = first_page(NULL, false);
	search = NULL;
	while (p)
	{
		if (p->format == format && (search = free_block_in_page(p, size)))
			break ;
		p = p->next;
	}
	if (search)
	{
		old_sz = search->size;
		set_block(search, size, false);
		if (old_sz - search->size > align(16, sizeof(t_block)))
			split_block(search, old_sz - search->size
					- align(16, sizeof(t_block)));
		return (search->ptr);
	}
	else
		return (new_page(format, size));
}
