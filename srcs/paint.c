/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paint.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 13:08:21 by bdevessi          #+#    #+#             */
/*   Updated: 2019/03/18 18:23:27 by bdevessi         ###   ########.fr       */
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

void	nothing_to_show_message(t_select *select)
{
	const int	middle_x = (select->window.ws_col - NOTHING_TO_SHOW_LEN - 1) / 2;
	const int	middle_y = (select->window.ws_row - 1) / 2;

	if (select->termcaps.mv_cursor)
		tputs(tgoto(select->termcaps.mv_cursor, middle_x, middle_y), 1, putchar_tty);
	putf_tty(WARNING "  " RED_FOREGROUD "%s" COLOR_RESET " " WARNING, NOTHING_TO_SHOW);
}

void	no_space_screen(t_select *select)
{
	size_t	x;
	size_t	y;

	if (select->termcaps.mv_cursor)
		tputs(tgoto(select->termcaps.mv_cursor, 1, 1), 1, putchar_tty);
	if (select->termcaps.clear)
		tputs(select->termcaps.clear, 1, putchar_tty);
	putf_tty(RED_BACKGROUND);
	y = 3;
	while (y++ < select->window.ws_col)
	{
		x = 0;
		while (x++ < select->window.ws_row)
			putchar_tty(' ');
	}
	putf_tty(COLOR_RESET);
}

bool	not_enough_space(t_select *select, int items_per_line)
{
	const int	nb_lines = 3 + select->selector.visible_count / items_per_line * ITEM_HEIGHT;

	return (nb_lines > select->window.ws_row || select->window.ws_col < 40);
}

bool	paint(t_item *items, t_select *select, t_search *search)
{
	const int	divisor = select->selector.max_item_text_len + ITEM_PADDING;
	const int	max_items_per_line = select->window.ws_col / divisor;
	t_box	box;
	size_t	i;
	size_t	item_id;

	if (not_enough_space(select, max_items_per_line))
	{
		no_space_screen(select);
		return (false);
	}
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
	if (select->selector.visible_count == 0)
		nothing_to_show_message(select);
	select->selector.dirty = false;
	return (true);
}
