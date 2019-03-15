/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/14 16:30:24 by bdevessi          #+#    #+#             */
/*   Updated: 2019/03/15 17:10:06 by bdevessi         ###   ########.fr       */
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

static bool	match(const char *s1, const char *s2)
{
	if (*s1 == *s2 && *s1 != '\0')
		return (match(s1 + 1, s2 + 1));
	if (*s2 == '*' && *s1 != '\0')
		return (match(s1 + 1, s2) || match(s1, s2 + 1));
	if (*s2 == '*' && *s1 == '\0')
		return (match(s1, s2 + 1));
	if (*s2 == '\0' && *s1 == '\0')
		return (true);
	return (false);
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
	if (select->selector.visible_count != count)
		select->selector.index = 0;
	select->selector.visible_count = search->len > 0 ? count : select->selector.len;
	select->selector.max_item_text_len = calculate_max_text_len_items(select->selector.items, select->selector.len);

}

static void	pad_end_color(const char *color, char *string, size_t len, size_t max)
{
	putf_tty("%s%s", color, string);
	while (len++ < max)
		putchar_tty(' ');
	putf_tty(CSI "0m");
}

void		paint_search_input(t_search *search, t_select *select)
{
	if (!(select->termcaps.mv_cursor))
		return ;
	tputs(tgoto(select->termcaps.mv_cursor, 9, select->window.ws_row), 1, putchar_tty);
	pad_end_color(GREY_COLOR, search->query, search->len, search->query_field_len);
	sort_items(search, select);
}

void		paint_search(t_search *search, t_select *select)
{
	if (!(select->termcaps.mv_cursor))
		return ;
	tputs(tgoto(select->termcaps.mv_cursor, 0, select->window.ws_row), 1, putchar_tty);
	putf_tty("Search : ");
	pad_end_color(GREY_COLOR, search->query, search->len, search->query_field_len);
	sort_items(search, select);
}
