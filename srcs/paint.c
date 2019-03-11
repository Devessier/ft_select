/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paint.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 13:08:21 by bdevessi          #+#    #+#             */
/*   Updated: 2019/03/11 15:39:59 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <term.h>

#include <stdio.h>

#include "select.h"
#include "libft.h"

bool	paint(t_item *items, t_select *select)
{
	const int	items_per_line = select->window.ws_col / (select->selector.max_item_text_len + ITEM_PADDING);
	size_t		i;
	int			x;
	int			y;

	if (select->termcaps.clear)
	{
		tputs(select->termcaps.clear, 1, &ft_putchar);
		putchar('\n');
	}
	(void)items;
	i = 0;
	x = 0;
	y = 0;
	while (i++ < select->selector.len)
	{
		if (items_per_line == 0 || (i - 1) % items_per_line == 0)
			x = ITEM_PADDING / 2;
		else
			x = ((i - 1) % items_per_line) * (select->selector.max_item_text_len + ITEM_PADDING / 2);
		y = (i - 1) / items_per_line;
		items[i - 1].paint(&items[i - 1], &select->termcaps, (t_coord) { x, y });
	}
	putchar('\n');
	sleep(1);
	return (true);
}
