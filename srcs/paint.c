/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paint.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 13:08:21 by bdevessi          #+#    #+#             */
/*   Updated: 2019/03/21 15:01:52 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <term.h>
#include "select.h"
#include "libft.h"

void		pad(size_t pad_of)
{
	while (pad_of--)
		putchar_tty(' ');
}

static bool	checks(t_select *select, t_search *search, int max_items_per_line)
{
	if (not_enough_space(select, max_items_per_line))
	{
		no_space_screen(select);
		select->overflow = true;
		return (false);
	}
	else
		select->overflow = false;
	select->selector.max_items_per_line = max_items_per_line;
	if (select->selector.dirty || search->dirty || g_resize)
	{
		if (select->termcaps.clear_scr)
			tputs(select->termcaps.clear_scr, 1, putchar_tty);
		print_header(select);
		paint_search(search, select);
	}
	return (true);
}

static void	paint_loop(t_select *select, t_item *items,
	int max_items_per_line, size_t i)
{
	t_box	box;
	size_t	item_id;
	size_t	x;

	item_id = 0;
	while (i++ < select->selector.len)
	{
		if (!((!items[i - 1].dirty && !select->selector.dirty)
			|| items[i - 1].hidden))
		{
			x = (item_id % max_items_per_line) *
					(select->selector.max_item_text_len + ITEM_PADDING) + 5;
			box = (t_box) {
				.x = x,
				.y = item_id / max_items_per_line + 1,
				.width = select->selector.max_item_text_len
			};
			items[i - 1].text_align = CENTER;
			items[i - 1].paint(&items[i - 1], &select->termcaps,
				box, select->selector.dirty);
		}
		if (!items[i - 1].hidden)
			item_id++;
	}
}

bool		paint(t_item *items, t_select *select, t_search *search)
{
	const int	divisor = select->selector.max_item_text_len + ITEM_PADDING;
	const int	max_items_per_line = select->window.ws_col / divisor;

	if (!checks(select, search, max_items_per_line))
		return (false);
	paint_loop(select, items, max_items_per_line, 0);
	if (select->selector.visible_count == 0)
		nothing_to_show_message(select);
	select->selector.dirty = false;
	return (true);
}
