/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: squiquem <squiquem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 17:28:34 by squiquem          #+#    #+#             */
/*   Updated: 2019/11/26 15:04:36 by squiquem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

/*
**	CONNECT_BLOCKS
**	Connects 3 blocks to each other
*/

void	connect_blocks(t_block *prev, t_block *block, t_block *next)
{
	block->next = next;
	block->prev = prev;
	if (next)
		next->prev = block;
	if (prev)
		prev->next = block;
}

/*
**	SET_BLOCK
**	Initializes block with size and free values
*/

void	set_block(t_block *block, size_t size, bool free)
{
	block->size = size;
	block->free = free;
	block->ptr = (void*)block + align(16, sizeof(t_block));
}

/*
**	SPLIT_BLOCK
**	Splits a block in 2
**	New block size = size & new block is free
*/

void	split_block(t_block *block, size_t size)
{
	t_block	*new_block;
	t_block	*next;

	if (size <= align(16, sizeof(t_block)))
		return ;
	new_block = (t_block*)(block->ptr + block->size);
	set_block(new_block, size, true);
	next = (block->next) ? block->next : NULL;
	connect_blocks(block, new_block, next);
}
