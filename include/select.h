/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   select.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 10:48:49 by bdevessi          #+#    #+#             */
/*   Updated: 2019/03/11 15:20:07 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SELECT_H
# define SELECT_H
# include <sys/types.h>
# include <sys/ioctl.h>
# define ITEM_PADDING 10

typedef struct	s_window
{
	size_t	width;
	size_t	height;
}				t_window;

typedef struct	s_coord
{
	int	x;
	int	y;
}				t_coord;

typedef struct	s_termcaps
{
	char	*sv_cursor;
	char	*re_cursor;
	char	*clear;
	char	*mv_cursor;
}				t_termcaps;

typedef struct	s_item
{
	char		*text;
	size_t		text_len;
	bool		selected;
	bool		hover;
	bool		(*paint)(struct s_item*, t_termcaps*, t_coord);
}				t_item;

typedef struct	s_elector
{
	size_t	index;
	size_t	len;
	t_item	*items;
	size_t	max_item_text_len;
}				t_selector;

typedef struct	s_elect
{
	struct winsize	window;
	t_termcaps		termcaps;
	t_selector		selector;
	bool			exit;
}				t_select;

typedef struct	s_winsize
{
	struct winsize	ws;
	bool			error;
}				t_winsize;

t_winsize		get_terminal_size(void);

bool			instanciate_items(t_selector *selector, int count, char **texts);


bool			paint(t_item *items, t_select *select);
bool			loop(int count, char **items);

extern bool		g_resize;

#endif
