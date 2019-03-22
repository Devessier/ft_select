/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_items.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/21 16:08:35 by bdevessi          #+#    #+#             */
/*   Updated: 2019/03/22 10:57:52 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include "select.h"
#include "libft.h"

bool		instanciate_items(t_selector *selector, int count, char **texts)
{
	t_item	*items;
	int		i;
	size_t	max_len;

	if (!(items = (t_item*)malloc(sizeof(t_item) * count)))
		return (false);
	g_items = items;
	i = 0;
	max_len = 0;
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

size_t		mtext_len(t_item *items, size_t len)
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
