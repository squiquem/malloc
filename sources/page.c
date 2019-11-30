/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   page.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: squiquem <squiquem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/20 23:58:10 by squiquem          #+#    #+#             */
/*   Updated: 2019/11/26 15:01:12 by squiquem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

/*
**	SET_PAGE
**	Sets new page values
*/

static void	set_page(int format, size_t size, t_page *prev)
{
	g_page->format = format;
	g_page->size = size;
	g_page->start = (t_block*)((void*)g_page + align(16, sizeof(t_page)));
	g_page->prev = prev;
	if (prev)
		prev->next = g_page;
	g_page->next = NULL;
}

/*
**	FIRST_PAGE
**	Using static variable to save first page of g_page
*/

t_page		*first_page(t_page *new, bool force)
{
	static t_page	*first = NULL;

	if (new || force)
	{
		first = new;
		if (first)
			first->prev = NULL;
	}
	return (first);
}

/*
**	GO_TO_LAST_PAGE
**	Puts last page of g_page to *prev
**	g_page = NULL
*/

void		go_to_last_page(t_page **prev)
{
	if (!g_page)
		return ;
	while (g_page->next)
		g_page = g_page->next;
	*prev = g_page;
	g_page = g_page->next;
}

/*
**	DELETE_PAGE
**	Links next and prev pages to each other before deleting
*/

void		delete_page(t_page *p)
{
	if (!p->prev)
	{
		g_page = p->next;
		first_page(g_page, true);
	}
	else if (!p->next)
		p->prev->next = NULL;
	else
	{
		p->prev->next = p->next;
		p->next->prev = p->prev;
	}
	munmap(p, p->size);
}

/*
**	NEW_PAGE
**	Creates a new page and links it to g_page
*/

void		*new_page(int format, size_t p_size)
{
	size_t	s;
	size_t	new_block_sz;
	t_page	*prev;

	s = align_page(format, p_size + align(16, sizeof(t_page))
			+ align(16, sizeof(t_block)));
	g_page = first_page(NULL, false);
	prev = NULL;
	go_to_last_page(&prev);
	if (!(g_page = mmap(0, s, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON,
					-1, 0)))
		return (NULL);
	set_page(format, s, prev);
	if (!prev)
		first_page(g_page, false);
	set_block(g_page->start, p_size, false);
	connect_blocks(NULL, g_page->start, NULL);
	new_block_sz = g_page->size - (align(16, sizeof(t_page))
			+ align(16, sizeof(t_block)) * 2 + p_size);
	if (format != LARGE)
		split_block(g_page->start, new_block_sz);
	return (g_page->start->ptr);
}
