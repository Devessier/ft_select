/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   item.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 12:41:56 by bdevessi          #+#    #+#             */
/*   Updated: 2019/03/22 10:41:28 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/ioctl.h>
#include <term.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "select.h"
#include "libft.h"

t_item	*g_items = NULL;

bool	paint_fn(t_item *item, t_termcaps *termcaps, t_box box, bool force)
{
	if ((!force && !item->dirty) || item->hidden)
		return (true);
	if (termcaps->mv_cursor)
		tputs(tgoto(termcaps->mv_cursor, box.x, box.y * ITEM_HEIGHT),
			1, putchar_tty);
	text_align(item, &box);
	if (item->hover && termcaps->underline)
		tputs(termcaps->underline, 1, putchar_tty);
	if (item->selected && termcaps->reverse_video)
		tputs(termcaps->reverse_video, 1, putchar_tty);
	color_item(item, termcaps);
	if ((item->hover || item->selected) && termcaps->disable_style)
		tputs(termcaps->disable_style, 1, putchar_tty);
	item->dirty = false;
	return (true);
}

void	init_item(t_item *item, char *text, size_t index)
{
	struct stat	buf;

	*item = (t_item) {
		.text = text,
		.text_len = ft_strlen(text),
		.selected = false,
		.hover = index == 0,
		.hidden = false,
		.suppressed = false,
		.paint = paint_fn,
		.dirty = false,
		.file_type = 0,
	};
	if (lstat(item->text, &buf) != -1)
		item->file_type = buf.st_mode;
}

t_item	*item_from_id(t_item *items, size_t len, int id)
{
	size_t	i;

	i = 0;
	while (i++ < len)
		if (items[i - 1].id == id && !items[i - 1].hidden)
			return (&items[i - 1]);
	return (NULL);
}
