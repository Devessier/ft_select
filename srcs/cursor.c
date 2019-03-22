/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cursor.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/22 11:16:54 by bdevessi          #+#    #+#             */
/*   Updated: 2019/03/22 11:57:58 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "select.h"
#include <stdlib.h>

static void	move_1(t_select *select, int index)
{
	if (index == 1)
	{
		if (select->selector.index == select->selector.visible_count - 1)
			select->selector.index = 0;
		else
			select->selector.index++;
	}
	else if (index == -1)
	{
		if (select->selector.index == 0)
			select->selector.index = select->selector.visible_count - 1;
		else
			select->selector.index--;
	}
}

static void	move_2(t_select *select, int index, int move)
{
	if (index == 0)
	{
		if ((ssize_t)select->selector.index + move < 0
		|| (ssize_t)select->selector.index + move
		> (ssize_t)select->selector.visible_count - 1)
			select->selector.index = (ssize_t)select->selector.index
				+ move < 0 ? select->selector.visible_count - 1 : 0;
		else
			select->selector.index += move;
	}
	else if (index == 2)
	{
		if ((ssize_t)select->selector.index + move < 0
		|| (ssize_t)select->selector.index + move
		> (ssize_t)select->selector.visible_count - 1)
			select->selector.index = (ssize_t)select->selector.index
				+ move < 0 ? 0 : select->selector.visible_count - 1;
		else
			select->selector.index += move;
	}
	else if (index != 1 && index != -1)
		select->selector.index = move;
}

void		move_cursor(t_select *select, int index, int move)
{
	t_item	*item;

	if ((item = item_from_id(select->selector.items,
		select->selector.len, select->selector.index)) != NULL)
		item->dirty = true;
	if (select->selector.visible_count == 0 && (index == 1 || index == -1))
		return ;
	move_1(select, index);
	move_2(select, index, move);
	if ((item = item_from_id(select->selector.items,
		select->selector.len, select->selector.index)) != NULL)
		item->dirty = true;
}

void		hover(t_select *select)
{
	size_t	i;

	i = 0;
	while (i++ < select->selector.len)
		select->selector.items[i - 1].hover = select->selector.items[i
		- 1].id == (int)select->selector.index;
}
