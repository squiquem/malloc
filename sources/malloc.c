/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: squiquem <squiquem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/20 23:04:35 by squiquem          #+#    #+#             */
/*   Updated: 2019/11/30 12:04:19 by squiquem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;

/*
**  MALLOC
**  The malloc() function allocates size bytes and returns a pointer to the
**  allocated memory. The memory is not initialized. If size is 0, then malloc()
**  returns either NULL, or a unique pointer value that can later be
**  successfully passed to free()
*/

void	*malloc(size_t size)
{
	int		format;
	void	*p;
	void	*f;

	pthread_mutex_lock(&g_mutex);
	size = align(16, size);
	format = get_format(size);
	if (!g_page || format == LARGE)
	{
		p = new_page(format, size);
		pthread_mutex_unlock(&g_mutex);
		return (ft_error(p));
	}
	f = free_block(format, size);
	pthread_mutex_unlock(&g_mutex);
	return (ft_error(f));
}

/*
**  CALLOC
**  The calloc() function contiguously allocates enough space for count objects
**  that are size bytes of memory each and returns a pointer to the allocated
**  memory.  The allocated memory is filled with bytes of value zero
*/

void	*calloc(size_t count, size_t size)
{
	size_t	full_size;
	void	*new;

	full_size = count * size;
	if (!(new = malloc(full_size)))
		return (new);
	if (full_size)
		ft_bzero(new, full_size);
	return (new);
}

/*
**  VALLOC
**  The valloc() function allocates size bytes of memory and returns a pointer
**	to the allocated memory.  The allocated memory is aligned on a page boundary
*/

void	*valloc(size_t size)
{
	int		format;
	void	*p;
	void	*f;

	pthread_mutex_lock(&g_mutex);
	size = align_page(LARGE, size);
	format = get_format(size);
	if (!g_page || format == LARGE)
	{
		p = new_page(format, size);
		pthread_mutex_unlock(&g_mutex);
		return (ft_error(p));
	}
	f = free_block(format, size);
	pthread_mutex_unlock(&g_mutex);
	return (ft_error(f));
}

/*
**  ALIGNED_ALLOC
**  The aligned_alloc() function allocates size bytes of memory with an
**  alignment specified by alignment and returns a pointer to the allocated
**  memory
*/

void	*aligned_alloc(size_t alignment, size_t size)
{
	int		format;
	void	*p;
	void	*f;

	pthread_mutex_lock(&g_mutex);
	size = align(alignment, size);
	format = get_format(size);
	if (!g_page || format == LARGE)
	{
		p = new_page(format, size);
		pthread_mutex_unlock(&g_mutex);
		return (ft_error(p));
	}
	f = free_block(format, size);
	pthread_mutex_unlock(&g_mutex);
	return (ft_error(f));
}
