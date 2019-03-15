/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paint.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 13:08:21 by bdevessi          #+#    #+#             */
/*   Updated: 2019/03/15 14:54:34 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <term.h>
#include "select.h"
#include "libft.h"

void	pad(size_t pad_of)
{
	while (pad_of--)
		putchar_tty(' ');
}

bool	paint(t_item *items, t_select *select, t_search *search)
{
	const int	max_items_per_line = select->window.ws_col / (select->selector.max_item_text_len + ITEM_PADDING);
	t_box	box;
	size_t	i;
	size_t	item_id;

	select->selector.max_items_per_line = max_items_per_line;
	if (select->selector.dirty || search->dirty)
	{
		if (select->termcaps.mv_cursor)
			tputs(tgoto(select->termcaps.mv_cursor, 1, 1), 1, putchar_tty);
		if (select->termcaps.clear)
			tputs(select->termcaps.clear, 1, putchar_tty);
		print_header(select);
		paint_search(search, select);
	}
	i = 0;
	item_id = 0;
	while (i++ < select->selector.len)
	{
		if (!((!items[i - 1].dirty && !select->selector.dirty) || items[i - 1].hidden)) 
		{
			box = (t_box) {
				.x = (item_id % max_items_per_line) * (select->selector.max_item_text_len + ITEM_PADDING) + 5,
				.y = item_id / max_items_per_line + 1,
				.width = select->selector.max_item_text_len
			};
			items[i - 1].text_align = CENTER;
			items[i - 1].paint(&items[i - 1], &select->termcaps, box, select->selector.dirty);
		}
		if (!items[i - 1].hidden)
			item_id++;
	}
	select->selector.dirty = false;
	return (true);
}
