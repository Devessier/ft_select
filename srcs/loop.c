/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 12:20:44 by bdevessi          #+#    #+#             */
/*   Updated: 2019/03/18 17:26:28 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/ioctl.h>
#include <stdbool.h>
#include <term.h>
#include <curses.h>
#include "select.h"
#include "libft.h"

bool			g_resize = true;

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
			.clear = tgetstr("cd", NULL),
			.clear_scr = tgetstr("cl", NULL),
			.mv_cursor = tgetstr("cm", NULL),
			.k_left = tgetstr("kl", NULL),
			.k_up = tgetstr("ku", NULL),
			.k_right = tgetstr("kr", NULL),
			.k_down = tgetstr("kd", NULL),
			.reverse_video = tgetstr("mr", NULL),
			.underline = tgetstr("us", NULL),
			.disable_style = tgetstr("me", NULL),
			.cur_visible = tgetstr("ve", NULL),
			.cur_invisible = tgetstr("vi", NULL),
			.cup_mode = tgetstr("ti", NULL),
			.stop_cup_mode = tgetstr("te", NULL),
		},
		.exit = false,
	};
	if (!instanciate_items(&select->selector, count, texts))
		return (false);
	return (true);
}

void		move_cursor(t_select *select, int index, int move)
{
	t_item	*item;

	if ((item = item_from_id(select->selector.items, select->selector.len, select->selector.index)) != NULL)
		item->dirty = true;
	if (select->selector.visible_count == 0 && (index == 1 || index == -1))
		return ;
	if (index == 1)
		if (select->selector.index == select->selector.visible_count - 1)
			select->selector.index = 0;
		else
			select->selector.index++;
	else if (index == -1)
		if (select->selector.index == 0)
			select->selector.index = select->selector.visible_count - 1;
		else
			select->selector.index--;
	else if (index == 0)
		if ((ssize_t)select->selector.index + move < 0 || (ssize_t)select->selector.index + move > (ssize_t)select->selector.visible_count - 1)
			select->selector.index = (ssize_t)select->selector.index + move < 0 ? select->selector.visible_count - 1 : 0;
		else
			select->selector.index += move;
	else if (index == 2)
		if ((ssize_t)select->selector.index + move < 0 || (ssize_t)select->selector.index + move > (ssize_t)select->selector.visible_count - 1)
			select->selector.index = (ssize_t)select->selector.index + move < 0 ? 0 : select->selector.visible_count - 1;
		else
			select->selector.index += move;
	else
		select->selector.index = move;
	if ((item = item_from_id(select->selector.items, select->selector.len, select->selector.index)) != NULL)
		item->dirty = true;
}

static int	handle_special_characters(char buffer[BUFF_SIZE], t_select *select, t_search *search)
{
	t_item	*item;

	item = item_from_id(select->selector.items, select->selector.len, select->selector.index);
	if (*buffer == 0xd && buffer[1] == 0)
		return (2);
	if (*buffer == 0x1b && buffer[1] == 0)
		return (-1);
	if ((*buffer == 0x7f || *buffer == 0x8) && buffer[1] == 0)
	{
		if (select->selector.visible_count == 0)
			return (1);
		item->suppressed = true;
		select->selector.dirty = true;
		if (select->selector.index == select->selector.visible_count - 1)
			move_cursor(select, 2, -1);
		return (modify_items(select) ? 1 : -1);
	}
	if (*buffer == ' ' && buffer[1] == 0)
	{
		if (select->selector.visible_count == 0)
			return (1);
		item->selected ^= 1;
		item->dirty = true;
		move_cursor(select, 0, 1);
		return (1);
	}
	if (*buffer == 0x1b && buffer[1] == '[')
	{
		if (buffer[2] == 'A' && select->selector.index + 1 > (size_t)select->selector.max_items_per_line)
			move_cursor(select, 2, -select->selector.max_items_per_line);
		else if (buffer[2] == 'B' && (int)(select->selector.visible_count - select->selector.index) > select->selector.max_items_per_line)

			move_cursor(select, 2, select->selector.max_items_per_line);
		else if (buffer[2] == 'C')
			move_cursor(select, 1, 0);
		else if (buffer[2] == 'D')
			move_cursor(select, -1, 0);
		else if (buffer[2] == 0x33 && buffer[3] == 0x7e && search->cursor > 0)
		{
			search->query[--search->cursor] = '\0';
			search->len--;
			return (0);
		}
		return (1);
	}
	else if (ft_isprint(*buffer))
	{
		search->query[search->cursor++] = *buffer;
		search->len++;
		return (0);
	}
	return (1);
}

static void	hover(t_select *select)
{
	size_t	i;

	i = 0;
	while (i++ < select->selector.len)
		select->selector.items[i - 1].hover = select->selector.items[i - 1].id == (int)select->selector.index;
}

static void	refresh_ui(t_select *select, t_search *search)
{
	if (search->dirty)
		paint_search_input(search, select);
	hover(select);
	paint(select->selector.items, select, search);
	search->dirty = false;
}

static bool	handle_resize(t_select *select, t_search *search)
{
	const t_winsize	winsize = get_terminal_size();

	if (winsize.error)
		return (false);	
	select->selector.dirty = true;
	select->window = winsize.ws;
	g_resize = false;
	refresh_ui(select, search);
	return (true);
}

bool		loop(int count, char **texts)
{
	char		buffer[BUFF_SIZE];
	t_select	select;
	t_search	search;
	ssize_t		nbytes;
	int			result;

	if (!init_select(&select, count, texts))
	{
		ft_putf("error\n");
		return (false);
	}
	init_search(&search, &select);
	if (select.termcaps.cup_mode)
		tputs(select.termcaps.cup_mode, 1, putchar_tty);
	if (select.termcaps.clear_scr)
		tputs(select.termcaps.clear_scr, 1, putchar_tty);
	if (select.termcaps.cur_invisible)
		tputs(select.termcaps.cur_invisible, 1, putchar_tty);
	while (!select.exit)
	{
		if (g_resize)
			handle_resize(&select, &search);
		ft_bzero(buffer, BUFF_SIZE);
		if ((nbytes = read(0, buffer, BUFF_SIZE)) == -1)
			return (false);
		else if (nbytes > 0)
		{
			if ((result = handle_special_characters(buffer, &select, &search)) == -1)
				select.exit = true;
			else if (result == 2)
			{
				if (select.termcaps.stop_cup_mode)
					tputs(select.termcaps.stop_cup_mode, 1, putchar_tty);
				print_items(select.selector.items, select.selector.len);
				select.exit = true;
			}
			else if (result == 0)
				search.dirty = true;
			if (result <= 1)
				refresh_ui(&select, &search);
		}
	}
	if (select.termcaps.stop_cup_mode)
		tputs(select.termcaps.stop_cup_mode, 1, putchar_tty);
	if (select.termcaps.cur_visible)
		tputs(select.termcaps.cur_visible, 1, putchar_tty);
	return (true);
}
