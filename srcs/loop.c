/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 12:20:44 by bdevessi          #+#    #+#             */
/*   Updated: 2019/03/11 15:27:05 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/ioctl.h>
#include <stdbool.h>
#include <term.h>
#include "select.h"
#include "libft.h"

bool		g_resize = true;

t_winsize		get_terminal_size(void)
{
	struct winsize	ws;

	if (ioctl(0, TIOCGWINSZ, &ws) == -1)
		return ((t_winsize) {
			.error = true
		});
	return ((t_winsize) {
		.error = false,
		.ws = ws,
	});
}

static bool	init_select(t_select *select, int count, char **texts)
{
	const t_winsize	winsize = get_terminal_size();

	if (winsize.error)
		return (false);	
	*select = (t_select) {
		.window = winsize.ws,
		.termcaps = {
			.sv_cursor = tgetstr("sc", NULL),
			.re_cursor = tgetstr("rc", NULL),
			.clear = tgetstr("cl", NULL),
			.mv_cursor = tgetstr("cm", NULL),
		},
		.exit = false,
	};
	if (!instanciate_items(&select->selector, count, texts))
		return (false);
	return (true);
}

bool		loop(int count, char **texts)
{
	t_select	select;
	bool		refresh;

	if (!init_select(&select, count, texts))
	{
		ft_putf("error\n");
		return (false);
	}
	refresh = true;
	while (!select.exit)
		if (refresh)
		{
			paint(select.selector.items, &select);
		}
	return (true);
}
