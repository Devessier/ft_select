/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/14 16:30:24 by bdevessi          #+#    #+#             */
/*   Updated: 2019/03/22 14:45:44 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <term.h>
#include "select.h"
#include "libft.h"

void		init_search(t_search *search, t_select *select)
{
	*search = (t_search) {
		.len = 0,
		.items_count = select->selector.len,
		.cursor = 0,
		.query_field_len = 30,
	};
	ft_bzero(search->query, sizeof(search->query));
}

static bool	predicate(t_item *item, t_search *search)
{
	if (ft_strchr(search->query, '*') != NULL)
		return (match(item->text, search->query));
	return (ft_strncmp(search->query, item->text, search->len) == 0);
}

void		sort_items(t_search *search, t_select *select)
{
	size_t	i;
	size_t	count;

	i = 0;
	count = 0;
	while (i++ < select->selector.len)
	{
		if (search->len == 0)
		{
			select->selector.items[i - 1].hidden = false;
			select->selector.items[i - 1].id = i - 1;
		}
		else if (!(select->selector.items[i
		- 1].hidden = !predicate(&select->selector.items[i - 1], search)))
			select->selector.items[i - 1].id = count++;
		else
			select->selector.items[i - 1].hidden = true;
		select->selector.items[i - 1].dirty = true;
	}
	if (search->len > 0 && select->selector.visible_count != count && count > 0)
		select->selector.index = 0;
	select->selector.visible_count = search->len > 0
		? count : select->selector.len;
	select->selector.max_item_text_len = mtext_len(select->selector.items,
		select->selector.len);
}
