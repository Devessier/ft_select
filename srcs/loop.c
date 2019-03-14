/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 12:20:44 by bdevessi          #+#    #+#             */
/*   Updated: 2019/03/14 18:40:48 by bdevessi         ###   ########.fr       */
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

static void	move_cursor(t_select *select, int index, int move)
{
	select->selector.items[select->selector.index].dirty = true;
	if (index == 1)
		if (select->selector.index == select->selector.len - 1)
			select->selector.index = 0;
		else
			select->selector.index++;
	else if (index == -1)
		if (select->selector.index == 0)
			select->selector.index = select->selector.len - 1;
		else
			select->selector.index--;
	else if (index == 0)
		if ((ssize_t)select->selector.index + move < 0 || (ssize_t)select->selector.index + move > (ssize_t)select->selector.len - 1)
			select->selector.index = (ssize_t)select->selector.index + move < 0 ? select->selector.len - 1 : 0;
		else
			select->selector.index += move;
	else if (index == 2)
		if ((ssize_t)select->selector.index + move < 0 || (ssize_t)select->selector.index + move > (ssize_t)select->selector.len - 1)
			select->selector.index = (ssize_t)select->selector.index + move < 0 ? 0 : select->selector.len - 1;
		else
			select->selector.index += move;
	else
		select->selector.index = move;
	select->selector.items[select->selector.index].dirty = true;
}

static int	handle_special_characters(char buffer[BUFF_SIZE], t_select *select, t_completion *completion)
{
	if (*buffer == 0xd && buffer[1] == 0)
		return (2);
	if (*buffer == 0x1b && buffer[1] == 0)
		return (-1);
	if ((*buffer == 0x7f || *buffer == 0x8) && buffer[1] == 0)
	{
		select->selector.items[select->selector.index].suppressed = true;
		select->selector.dirty = true;
		move_cursor(select, 2, -1);
		return (modify_items(select) ? 1 : -1);
	}
	if (*buffer == ' ' && buffer[1] == 0)
	{
		select->selector.items[select->selector.index].selected ^= 1;
		select->selector.items[select->selector.index].dirty = true;
		move_cursor(select, 0, 1);
		return (1);
	}
	if (*buffer == 0x1b && buffer[1] == '[')
	{
		if (buffer[2] == 'A' && select->selector.index + 1 > (size_t)select->selector.max_items_per_line)
			move_cursor(select, 2, -select->selector.max_items_per_line);
		else if (buffer[2] == 'B' && select->selector.index < (select->selector.len - select->selector.max_items_per_line))
			move_cursor(select, 2, select->selector.max_items_per_line);
		else if (buffer[2] == 'C')
			move_cursor(select, 1, 0);
		else if (buffer[2] == 'D')
			move_cursor(select, -1, 0);
		else
			return (0);
		return (1);
	}
	else
	{
		ft_putf_fd(2, "cursor = %d\n", completion->cursor);
		completion->query[completion->cursor++] = *buffer;
		ft_putf_fd(2, "s = %s\n", completion->query);
		completion->len++;
		return (0);
	}
	return (1);
}

static void	modify_list(t_select *select)
{
	size_t	i;

	i = 0;
	while (i++ < select->selector.len)
		if (i - 1 == select->selector.index)
			select->selector.items[i - 1].hover = true;
		else
			select->selector.items[i - 1].hover = false;
}

bool		handle_resize(t_select *select)
{

	const t_winsize	winsize = get_terminal_size();

	if (winsize.error)
		return (false);	
	select->selector.dirty = true;
	select->window = winsize.ws;
	g_resize = false;
	return (true);
}

bool		loop(int count, char **texts)
{
	char			buffer[BUFF_SIZE];
	t_select		select;
	t_completion	completion;
	bool			refresh;
	ssize_t			nbytes;
	int				result;

	if (!init_select(&select, count, texts))
	{
		ft_putf("error\n");
		return (false);
	}
	init_completion(&completion, &select);
	if (select.termcaps.cup_mode)
		tputs(select.termcaps.cup_mode, 1, putchar_tty);
	if (select.termcaps.clear_scr)
		tputs(select.termcaps.clear_scr, 1, putchar_tty);
	if (select.termcaps.cur_invisible)
		tputs(select.termcaps.cur_invisible, 1, putchar_tty);
	refresh = true;
	while (!select.exit)
	{
		if (g_resize)
			handle_resize(&select);
		ft_bzero(buffer, BUFF_SIZE);
		if ((nbytes = read(0, buffer, BUFF_SIZE)) == -1)
			return (false);
		else if (nbytes > 0)
		{
			if ((result = handle_special_characters(buffer, &select, &completion)) == -1)
				select.exit = true;
			else if (result == 1)
				refresh = true;
			else if (result == 2)
			{
				if (select.termcaps.stop_cup_mode)
					tputs(select.termcaps.stop_cup_mode, 1, putchar_tty);
				print_items(select.selector.items, select.selector.len);
				select.exit = true;
			}
			else if (result == 0)
			{
				select.selector.dirty = true;
				refresh = true;
				paint_completion(&completion, &select);
			}
		}
		if (refresh)
		{
			modify_list(&select);
			paint(select.selector.items, &select, &completion);
			refresh = false;
		}
	}
	if (select.termcaps.stop_cup_mode)
		tputs(select.termcaps.stop_cup_mode, 1, putchar_tty);
	return (true);
}
