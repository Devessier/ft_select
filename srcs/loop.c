/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 12:20:44 by bdevessi          #+#    #+#             */
/*   Updated: 2019/03/22 11:59:10 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/ioctl.h>
#include <stdbool.h>
#include <termcap.h>
#include "select.h"
#include "libft.h"

bool			g_resize = true;

static void		refresh_ui(t_select *select, t_search *search)
{
	if (search->dirty)
		paint_search_input(search, select);
	hover(select);
	paint(select->selector.items, select, search);
	search->dirty = false;
}

static bool		handle_resize(t_select *select, t_search *search)
{
	const t_winsize	winsize = get_terminal_size();

	if (winsize.error)
		return (false);
	select->selector.dirty = true;
	select->window = winsize.ws;
	refresh_ui(select, search);
	g_resize = false;
	return (true);
}

static bool		init_loop(int count, char **texts
	, t_select *select, t_search *search)
{
	if (!init_select(select, count, texts))
	{
		ft_putf_fd(2, "ft_select: fatal error, exit\n");
		return (false);
	}
	init_search(search, select);
	if (select->termcaps.cup_mode)
		tputs(select->termcaps.cup_mode, 1, putchar_tty);
	if (select->termcaps.clear_scr)
		tputs(select->termcaps.clear_scr, 1, putchar_tty);
	if (select->termcaps.cur_invisible)
		tputs(select->termcaps.cur_invisible, 1, putchar_tty);
	return (true);
}

static void		for_each(t_select *select, t_search *search,
	char buffer[BUFF_SIZE], bool *return_state)
{
	ssize_t	nbytes;
	int		result;

	if ((nbytes = read(0, buffer, BUFF_SIZE)) == -1)
	{
		*return_state = false;
		select->exit = true;
	}
	else if (nbytes > 0)
	{
		if ((result = handle_special_characters(buffer, select, search)) == -1)
			select->exit = true;
		else if (result == 2)
		{
			if (select->termcaps.stop_cup_mode)
				tputs(select->termcaps.stop_cup_mode, 1, putchar_tty);
			print_items(select->selector.items, select->selector.len);
			select->exit = true;
		}
		else if (result == 0)
			search->dirty = true;
		if (result <= 1)
			refresh_ui(select, search);
	}
}

bool			loop(int count, char **texts)
{
	char		buffer[BUFF_SIZE];
	t_select	select;
	t_search	search;
	bool		return_state;

	if (!init_loop(count, texts, &select, &search))
		return (false);
	return_state = true;
	while (!select.exit)
	{
		if (g_resize)
			handle_resize(&select, &search);
		ft_bzero(buffer, sizeof(buffer));
		for_each(&select, &search, buffer, &return_state);
	}
	if (select.termcaps.stop_cup_mode)
		tputs(select.termcaps.stop_cup_mode, 1, putchar_tty);
	if (select.termcaps.cur_visible)
		tputs(select.termcaps.cur_visible, 1, putchar_tty);
	free(g_items);
	return (return_state);
}
