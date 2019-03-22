/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paint_search.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/22 11:00:40 by bdevessi          #+#    #+#             */
/*   Updated: 2019/03/22 11:04:44 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <termcap.h>
#include "select.h"

static void	pad_end_color(const char *color, char *string,
	size_t len_cap[2], t_termcaps *termcaps)
{
	char	*tmp;

	tmp = string;
	if (len_cap[0] > 30)
		while (len_cap[0] > 30)
		{
			len_cap[0]--;
			tmp++;
		}
	if (termcaps->maximum_colors > 0)
		putf_tty("%s%s", color, tmp);
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
	tputs(tgoto(select->termcaps.mv_cursor, 9,
		select->window.ws_row), 1, putchar_tty);
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
	tputs(tgoto(select->termcaps.mv_cursor, 0,
		select->window.ws_row), 1, putchar_tty);
	putf_tty("Search : ");
	*len_cap = search->len;
	len_cap[1] = search->query_field_len;
	pad_end_color(GREY_COLOR, search->query, len_cap, &select->termcaps);
	sort_items(search, select);
}
