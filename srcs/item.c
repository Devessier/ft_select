/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   item.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 12:41:56 by bdevessi          #+#    #+#             */
/*   Updated: 2019/03/11 17:33:34 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/ioctl.h>
#include <term.h>
#include <stdbool.h>
#include <stdlib.h>

#include <stdio.h>

#include "select.h"
#include "libft.h"

#define CSI "\x1b["

static bool	paint_fn(t_item *item, t_termcaps *termcaps, t_coord coords)
{
	(void)item, (void)coords;
	if (termcaps->mv_cursor)
		tputs(tgoto(termcaps->mv_cursor, coords.x, coords.y * ITEM_HEIGHT), 1, &ft_putchar);
	ft_putf(CSI "91m" "%s" CSI "0m", item->text);
	return (true);
}

static void	init_item(t_item *item, char *text, size_t index)
{
	*item = (t_item) {
		.text = text,
		.text_len = ft_strlen(text),
		.selected = false,
		.hover = index == 0,
		.paint = paint_fn,
	};
}

bool		instanciate_items(t_selector *selector, int count, char **texts)
{
	t_item	*items;
	int		i;
	size_t	max_len;

	if (!(items = (t_item*)malloc(sizeof(t_item) * count)))
		return (false);
	i = 0;
	while (i++ < count)
	{
		init_item(&items[i - 1], texts[i - 1], i - 1);
		if (items[i - 1].text_len > max_len)
			max_len = items[i - 1].text_len;
	}
	*selector = (t_selector) {
		.index = 0,
		.len = count,
		.items = items,
		.max_item_text_len = max_len,
	};
	return (true);
}
