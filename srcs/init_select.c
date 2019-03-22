/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_select.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/22 11:26:32 by bdevessi          #+#    #+#             */
/*   Updated: 2019/03/22 11:58:49 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <termcap.h>
#include <stdlib.h>
#include "select.h"

t_winsize	get_terminal_size(void)
{
	struct winsize	ws;

	if (ioctl(0, TIOCGWINSZ, &ws) == -1)
	{
		return ((t_winsize) {
			.error = true
		});
	}
	return ((t_winsize) {
		.error = false,
		.ws = ws,
	});
}

static void	init_termcaps(t_select *select, const t_winsize winsize)
{
	*select = (t_select) {
		.window = winsize.ws,
		.termcaps = {
			.clear = tgetstr("cd", NULL),
			.clear_scr = tgetstr("cl", NULL),
			.mv_cursor = tgetstr("cm", NULL),
			.reverse_video = tgetstr("mr", NULL),
			.underline = tgetstr("us", NULL),
			.disable_style = tgetstr("me", NULL),
			.cur_visible = tgetstr("ve", NULL),
			.cur_invisible = tgetstr("vi", NULL),
			.cup_mode = tgetstr("ti", NULL),
			.stop_cup_mode = tgetstr("te", NULL),
			.maximum_colors = tgetnum("Co"),
		},
		.exit = false,
		.overflow = false,
	};
}

bool		init_select(t_select *select, int count, char **texts)
{
	const t_winsize	winsize = get_terminal_size();

	if (winsize.error)
		return (false);
	init_termcaps(select, winsize);
	if (!(select->termcaps.clear && select->termcaps.clear_scr
		&& select->termcaps.mv_cursor && select->termcaps.reverse_video
		&& select->termcaps.underline && select->termcaps.disable_style))
		return (false);
	if (!instanciate_items(&select->selector, count, texts))
		return (false);
	return (true);
}
