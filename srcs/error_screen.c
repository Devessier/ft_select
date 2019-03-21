/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_screen.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/21 14:21:55 by bdevessi          #+#    #+#             */
/*   Updated: 2019/03/21 14:22:37 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <termcap.h>
#include "select.h"

void	nothing_to_show_message(t_select *select)
{
	int	middle_x;
	int	middle_y;

	middle_x = (select->window.ws_col - NOTHING_TO_SHOW_LEN - 1) / 2;
	middle_y = (select->window.ws_row - 1) / 2;
	if (select->termcaps.mv_cursor)
		tputs(tgoto(select->termcaps.mv_cursor, middle_x, middle_y),
			1, putchar_tty);
	putf_tty(WARNING "  " RED_FOREGROUD "%s" COLOR_RESET
		" " WARNING, NOTHING_TO_SHOW);
}

void	no_space_screen(t_select *select)
{
	if (select->termcaps.mv_cursor)
		tputs(tgoto(select->termcaps.mv_cursor, 0, 0), 0, putchar_tty);
	if (select->termcaps.clear)
		tputs(select->termcaps.clear, 1, putchar_tty);
	if (select->termcaps.mv_cursor)
		tputs(tgoto(select->termcaps.mv_cursor, 0, 0), 0, putchar_tty);
	putf_tty(RED_BACKGROUND);
	if (select->termcaps.clear)
		tputs(select->termcaps.clear, 1, putchar_tty);
	putf_tty(COLOR_RESET);
}

bool	not_enough_space(t_select *select, int items_per_line)
{
	int		nb_lines;
	bool	error;

	if (items_per_line > 0)
	{
		nb_lines = 3 + select->selector.visible_count / items_per_line
			* ITEM_HEIGHT;
		error = nb_lines >= select->window.ws_row || select->window.ws_col
			<= 40;
	}
	else
		error = true;
	if (!select->selector.dirty)
		select->selector.dirty = error;
	return (error);
}
