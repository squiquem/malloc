/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_alloc_mem.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: squiquem <squiquem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/25 02:19:08 by squiquem          #+#    #+#             */
/*   Updated: 2019/11/26 15:00:17 by squiquem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

/*
**	PRINT_HEXA
**	Prints n as hexadecimal number without using malloc
*/

static void	print_hexa(size_t n)
{
	char	*str;
	size_t	i;
	size_t	tmp;
	char	buf[32];

	str = "0123456789ABCDEF";
	tmp = n;
	i = 3;
	while ((tmp /= 16))
		i++;
	buf[i] = 0;
	while (i--)
	{
		buf[i] = str[n % 16];
		n /= 16;
	}
	buf[0] = '0';
	buf[1] = 'x';
	ft_putstr(buf);
}

/*
**	PRINT_BLOCKS
**	Prints blocks allocations in one page
*/

static void	print_blocks(t_block *block, size_t *size)
{
	while (block)
	{
		if (!block->free)
		{
			print_hexa((size_t)block->ptr);
			ft_putstr(" - ");
			print_hexa((size_t)((void*)block->ptr + block->size));
			ft_putstr(" : ");
			ft_putnbr(block->size);
			ft_putstr(" octets\n");
			*size += block->size;
		}
		block = block->next;
	}
}

/*
**	PRINT_PAGE
**	Prints page allocations
*/

static void	print_page(t_page *p, int format, char *s, size_t *size)
{
	ft_putstr(s);
	ft_putstr(" : ");
	while (p && p->format != format)
		p = p->next;
	print_hexa((size_t)p);
	ft_putstr("\n");
	if (!p)
		return ;
	while (p)
	{
		if (p->format == format)
			print_blocks(p->start, size);
		p = p->next;
	}
}

/*
**	SHOW_ALLOC_MEM
*/

void		show_alloc_mem(void)
{
	t_page	*cur;
	size_t	size;

	pthread_mutex_lock(&g_mutex);
	size = 0;
	if (!(cur = first_page(NULL, false)))
	{
		ft_putstr("No allocation found\n");
		pthread_mutex_unlock(&g_mutex);
		return ;
	}
	print_page(cur, TINY, "TINY", &size);
	print_page(cur, SMALL, "SMALL", &size);
	print_page(cur, LARGE, "LARGE", &size);
	ft_putstr("Total:\n");
	ft_putnbr(size);
	ft_putstr(" octets\n");
	pthread_mutex_unlock(&g_mutex);
}
