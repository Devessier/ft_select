/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autocomplete.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/14 16:30:24 by bdevessi          #+#    #+#             */
/*   Updated: 2019/03/14 18:40:13 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <term.h>
#include "select.h"
#include "libft.h"

void		init_completion(t_completion *completion, t_select *select)
{
	*completion = (t_completion) {
		.len = 0,
		.items_count = select->selector.len,
		.cursor = 0,
		.query_field_len = 30,
	};
	ft_bzero(completion->query, sizeof(completion->query));
}

void		sort_items(t_completion *completion, t_select *select)
{
	size_t	i;

	if (completion->len == 0)
		return ;
	i = 0;
	while (i++ < select->selector.len)
	{
		select->selector.items[i - 1].hidden = ft_strncmp(completion->query, select->selector.items[i - 1].text, completion->len) != 0;	
		select->selector.items[i - 1].dirty = true;
	}
}

static void	pad_end_color(const char *color, char *string, size_t len, size_t max)
{
	putf_tty("%s%s", color, string);
	while (len++ < max)
		putchar_tty(' ');
	putf_tty(CSI "0m");
}

void		paint_completion(t_completion *completion, t_select *select)
{
	(void)completion;
	if (!(select->termcaps.mv_cursor))
		return ;
	tputs(tgoto(select->termcaps.mv_cursor, 0, select->window.ws_row), 1, putchar_tty);
	putf_tty("Search : ");
	pad_end_color(GREY_COLOR, completion->query, completion->len, completion->query_field_len);
	sort_items(completion, select);
}
