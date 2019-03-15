/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   item.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 12:41:56 by bdevessi          #+#    #+#             */
/*   Updated: 2019/03/15 17:59:25 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/ioctl.h>
#include <term.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "select.h"
#include "libft.h"

#define CSI "\x1b["

static void	text_align(t_item *item, t_box *box)
{
	const int	unused_space = box->width - item->text_len;

	if (item->text_align == RIGHT)
		pad(unused_space);
	else if (item->text_align == CENTER)
		pad(unused_space / 2);
}

static void	color_item(t_item *item)
{
	char	*color;

	color = NULL;
	if (S_ISFIFO(item->file_type))
		color = COLOR_FIFO;
	else if (S_ISCHR(item->file_type))
		color = COLOR_CHR;
	else if (S_ISBLK(item->file_type))
		color = COLOR_BLK;
	else if (S_ISDIR(item->file_type))
		color = COLOR_DIR;
	else if (S_ISLNK(item->file_type))
		color = COLOR_LNK;
	else if (S_ISSOCK(item->file_type))
		color = COLOR_SOCK;
	else if (S_ISREG(item->file_type))
		color = GREEN_COLOR;
	if (color != NULL)
		putf_tty("%s%s" CSI "0m", color, item->text);
	else
		putf_tty("%s", item->text);
}

static bool	paint_fn(t_item *item, t_termcaps *termcaps, t_box box, bool force)
{
	if ((!force && !item->dirty) || item->hidden)
		return (true);
	if (termcaps->mv_cursor)
		tputs(tgoto(termcaps->mv_cursor, box.x, box.y * ITEM_HEIGHT), 1, putchar_tty);
	text_align(item, &box);
	if (item->hover && termcaps->underline)
		tputs(termcaps->underline, 1, putchar_tty);
	if (item->selected && termcaps->reverse_video)
		tputs(termcaps->reverse_video, 1, putchar_tty);
	color_item(item);
	if ((item->hover || item->selected) && termcaps->disable_style)
		tputs(termcaps->disable_style, 1, putchar_tty);
	item->dirty = false;
	return (true);
}

static void	init_item(t_item *item, char *text, size_t index)
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
		items[i - 1].id = i - 1;
	}
	*selector = (t_selector) {
		.index = 0,
		.len = count,
		.items = items,
		.max_item_text_len = max_len,
		.dirty = true,
		.visible_count = count,
	};
	return (true);
}

size_t		calculate_max_text_len_items(t_item *items, size_t len)
{
	size_t	max;
	size_t	i;

	i = 0;
	max = 0;
	while (i++ < len)
		if (!items[i - 1].hidden && items[i - 1].text_len > max)
			max = items[i - 1].text_len;
	return (max);
}

bool		modify_items(t_select *select)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (i++ < select->selector.len)
		if (select->selector.items[i - 1].suppressed)
		{
			j = i - 1;
			while (j++ < select->selector.len - 1)
			{
				select->selector.items[j - 1] = select->selector.items[j];
				select->selector.items[j - 1].id--;
			}
			select->selector.len--;
		}
	if (select->selector.len == 0)
		return (false);
	return (true);
}

bool		print_items(t_item *items, size_t len)
{
	size_t	i;
	bool	endl;
	size_t	selected;

	selected = 0;
	i = 0;
	while (i++ < len)
		if (items[i - 1].selected && !items[i - 1].hidden)
			selected++;
	endl = false;
	i = 0;
	while (i++ < len)
	{
		if (!items[i - 1].selected || items[i - 1].hidden)
			continue ;
		ft_putstr(items[i - 1].text);
		if (--selected)
			ft_putchar(' ');
		endl = true;
	}
	return (true);
}

t_item		*item_from_id(t_item *items, size_t len, int id)
{
	size_t	i;

	i = 0;
	while (i++ < len)
		if (items[i - 1].id == id && !items[i - 1].hidden)
			return (&items[i - 1]);
	return (NULL);
}
