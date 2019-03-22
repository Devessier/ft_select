/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/22 11:10:36 by bdevessi          #+#    #+#             */
/*   Updated: 2019/03/22 11:15:21 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "select.h"
#include "libft.h"

static int	handle_del(t_item *item, t_select *select)
{
	if (select->selector.visible_count == 0)
		return (1);
	item->suppressed = true;
	select->selector.dirty = true;
	if (select->selector.index == select->selector.visible_count - 1)
		move_cursor(select, 2, -1);
	return (modify_items(select) ? 1 : -1);
}

static int	handle_space(t_item *item, t_select *select)
{
	if (select->selector.visible_count != 0)
	{
		item->selected ^= 1;
		item->dirty = true;
		move_cursor(select, 0, 1);
	}
	return (1);
}

static int	handle_keys(char buffer[BUFF_SIZE],
	t_select *select, t_search *search)
{
	if (buffer[2] == 'A' && select->selector.index + 1
	> (size_t)select->selector.max_items_per_line)
		move_cursor(select, 2, -select->selector.max_items_per_line);
	else if (buffer[2] == 'B'
	&& (int)(select->selector.visible_count - select->selector.index)
	> select->selector.max_items_per_line)
		move_cursor(select, 2, select->selector.max_items_per_line);
	else if (buffer[2] == 'C')
		move_cursor(select, 1, 0);
	else if (buffer[2] == 'D')
		move_cursor(select, -1, 0);
	else if (buffer[2] == 0x33 && buffer[3] == 0x7e
		&& search->cursor > 0 && search->len < 1024)
	{
		search->query[--search->cursor] = '\0';
		search->len--;
		return (0);
	}
	return (1);
}

int			handle_special_characters(char buffer[BUFF_SIZE],
	t_select *select, t_search *search)
{
	t_item	*item;

	item = item_from_id(select->selector.items,
		select->selector.len, select->selector.index);
	if (*buffer == 0xd && buffer[1] == 0)
		return (2);
	if (*buffer == 0x1b && buffer[1] == 0)
		return (-1);
	if (select->overflow)
		return (3);
	if ((*buffer == 0x7f || *buffer == 0x8) && buffer[1] == 0)
		return (handle_del(item, select));
	if (*buffer == ' ' && buffer[1] == 0)
		return (handle_space(item, select));
	if (*buffer == 0x1b && buffer[1] == '[')
		return (handle_keys(buffer, select, search));
	if (ft_isprint(*buffer))
	{
		search->query[search->cursor++] = *buffer;
		search->len++;
		return (0);
	}
	return (1);
}
