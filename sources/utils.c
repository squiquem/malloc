/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: squiquem <squiquem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/20 23:52:31 by squiquem          #+#    #+#             */
/*   Updated: 2019/11/26 14:59:28 by squiquem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

/*
**	GET_FORMAT
**	Returns the format int according to the size
*/

int			get_format(size_t size)
{
	if (size <= TINY_LIM)
		return (TINY);
	else if (size <= SMALL_LIM)
		return (SMALL);
	return (LARGE);
}

/*
**	LOG_PAGE
**	Returns log2(n)
*/

static int	log2(size_t n)
{
	int	cnt;

	cnt = 0;
	while (n > 1)
	{
		n >>= 1;
		cnt++;
	}
	return (cnt);
}

/*
**	ALIGN
**	Returns the alignment multiple number directly above size
*/

size_t		align(size_t alignment, size_t size)
{
	int	log;

	if (!size)
		return (alignment);
	log = log2(alignment);
	return ((((size - 1) >> log) << log) + alignment);
}

/*
**	ALIGN_PAGE
**	Returns the page_size multiple number directly above size
*/

size_t		align_page(int format, size_t size)
{
	size_t	page_size;

	if (format == TINY)
		page_size = TINY_COEF * getpagesize();
	else if (format == SMALL)
		page_size = SMALL_COEF * getpagesize();
	else
		page_size = getpagesize();
	return (align(page_size, size));
}
