/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: squiquem <squiquem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/22 00:34:11 by squiquem          #+#    #+#             */
/*   Updated: 2019/11/30 12:04:27 by squiquem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

/*
**	ATTACH_NEXT
**	Merges block and next block
*/

void		attach_to_next(t_block *block)
{
	block->size = block->size + block->next->size + align(16, sizeof(t_block));
	block->next = block->next->next;
	if (block->next)
		block->next->prev = block;
}

/*
**	DEFRAG
**	Merges prev and next blocks to block if free
*/

static void	defrag(t_block *block)
{
	block->free = true;
	if (block->prev && block->prev->free)
		attach_to_next(block->prev);
	if (block->next && block->next->free)
		attach_to_next(block);
}

/*
**	FREE
**	The free() function frees the memory space pointed to by ptr, which must
**	have been returned by a previous call to malloc(), calloc() or realloc().
**	Otherwise, or if free(ptr) has already been called before, undefined
**	behavior occurs. If ptr is NULL, no operation is performed.
*/

void		free(void *ptr)
{
	t_block	*block;
	t_page	*p;

	pthread_mutex_lock(&g_mutex);
	if (!ptr)
	{
		pthread_mutex_unlock(&g_mutex);
		return ;
	}
	p = first_page(NULL, false);
	if ((block = find_block(ptr, &p)))
		defrag(block);
	else
	{
		pthread_mutex_unlock(&g_mutex);
		return ;
	}
	if (p->start->free && !p->start->next)
		delete_page(p);
	pthread_mutex_unlock(&g_mutex);
}
