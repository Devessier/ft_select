/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paint.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 13:08:21 by bdevessi          #+#    #+#             */
/*   Updated: 2019/03/11 17:32:36 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <term.h>
#include "select.h"
#include "libft.h"

bool	paint(t_item *items, t_select *select)
{
	const int	max_items_per_line = select->window.ws_col / (select->selector.max_item_text_len + ITEM_PADDING);
	const int	flex_width = select->window.ws_col / (select->selector.len % max_items_per_line);
	const int	nb_lines = select->selector.len / max_items_per_line;
	size_t		i;
	int			x;
	int			y;

	i = 0;
	while (i++ < select->selector.len)
	{
		if ((int)i - 1 > max_items_per_line * nb_lines)
			x = ((i - 1) % max_items_per_line) * flex_width + (flex_width - select->selector.max_item_text_len) / 2;
		else
			x = ((i - 1) % max_items_per_line) * (select->selector.max_item_text_len + ITEM_PADDING) + 5;
		y = (i - 1) / max_items_per_line + 1;
		items[i - 1].paint(&items[i - 1], &select->termcaps, (t_coord) { x, y });
	}
	ft_putchar('\n');
	sleep(1);
	return (true);
}
