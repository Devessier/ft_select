/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/14 16:30:24 by bdevessi          #+#    #+#             */
/*   Updated: 2019/03/21 23:40:01 by Devessier        ###   ########.fr       */
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
		else if (!(select->selector.items[i - 1].hidden = !predicate(&select->selector.items[i - 1], search)))
			select->selector.items[i - 1].id = count++;
		else
			select->selector.items[i - 1].hidden = true;
		select->selector.items[i - 1].dirty = true;
	}
	if (search->len > 0 && select->selector.visible_count != count)
		select->selector.index = 0;
	select->selector.visible_count = search->len > 0 ? count : select->selector.len;
	select->selector.max_item_text_len = calculate_max_text_len_items(select->selector.items, select->selector.len);
}

static void	pad_end_color(const char *color, char *string, size_t len_cap[2], t_termcaps *termcaps)
{
	if (termcaps->maximum_colors > 0)
		putf_tty("%s%s", color, string);
	else
		putf_tty(string);
	while (len_cap[0]++ < len_cap[1])
		putchar_tty(' ');
	if (termcaps->maximum_colors > 0)
		putf_tty(CSI "0m");
}

void		paint_search_input(t_search *search, t_select *select)
{
	size_t	len_cap[2];

	if (!(select->termcaps.mv_cursor))
		return ;
	tputs(tgoto(select->termcaps.mv_cursor, 9, select->window.ws_row), 1, putchar_tty);
	*len_cap = search->len;
	len_cap[1] = search->query_field_len;
	pad_end_color(GREY_COLOR, search->query, len_cap, &select->termcaps);
	sort_items(search, select);
}

void		paint_search(t_search *search, t_select *select)
{
	size_t	len_cap[2];

	if (!(select->termcaps.mv_cursor))
		return ;
	tputs(tgoto(select->termcaps.mv_cursor, 0, select->window.ws_row), 1, putchar_tty);
	putf_tty("Search : ");
	*len_cap = search->len;
	len_cap[1] = search->query_field_len;
	pad_end_color(GREY_COLOR, search->query, len_cap, &select->termcaps);
	sort_items(search, select);
}
