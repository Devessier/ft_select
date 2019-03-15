/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   select.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 10:48:49 by bdevessi          #+#    #+#             */
/*   Updated: 2019/03/15 16:43:24 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SELECT_H
# define SELECT_H
# include <sys/types.h>
# include <sys/ioctl.h>
# include <sys/stat.h>
# include <stdbool.h>
# define ITEM_PADDING 10
# define ITEM_HEIGHT 2
# define READER_SIZE 4096
# define BUFF_SIZE 16
# define CSI "\x1b["
# define GREEN_COLOR CSI "92m"
# define BLUE_COLOR CSI "94m"
# define GREY_COLOR CSI "30;47m"
# define COLOR_RESET CSI "0m"
# define COLOR_FIFO CSI "33m"
# define COLOR_CHR CSI "34;103m"
# define COLOR_BLK CSI "34;106m"
# define COLOR_DIR CSI "36m"
# define COLOR_LNK CSI "35m"
# define COLOR_SOCK CSI "32m"

typedef struct	s_box
{
	int	x;
	int	y;
	int	width;
	int	height;
}				t_box;

typedef struct	s_termcaps
{
	char	*sv_cursor;
	char	*re_cursor;
	char	*clear;
	char	*clear_scr;
	char	*mv_cursor;
	char	*k_left;
	char	*k_up;
	char	*k_right;
	char	*k_down;
	char	*reverse_video;
	char	*underline;
	char	*disable_style;
	char	*cur_visible;
	char	*cur_invisible;
	char	*cup_mode;
	char	*stop_cup_mode;
}				t_termcaps;

typedef enum	s_text_align
{
	LEFT,
	CENTER,
	RIGHT
}				t_ext_align;

typedef struct	s_item
{
	int			id;
	char		*text;
	size_t		text_len;
	bool		selected;
	bool		hover;
	bool		hidden;
	bool		suppressed;
	t_ext_align	text_align;
	bool		(*paint)(struct s_item*, t_termcaps*, t_box, bool);
	bool		dirty;
	mode_t		file_type;
}				t_item;

typedef struct	s_earch
{
	char	query[1024];
	size_t	len;
	size_t	items_count;
	size_t	cursor;
	size_t	query_field_len;
	bool	dirty;
}				t_search;

typedef struct	s_elector
{
	size_t	index;
	size_t	len;
	size_t	visible_count;
	t_item	*items;
	size_t	max_item_text_len;
	int		max_items_per_line;
	bool	dirty;
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

typedef struct	s_reader
{
	char	buffer[READER_SIZE];
	size_t	len;
	size_t	index;
}				t_reader;


int				fd(void);

void			init_search(t_search *search, t_select *select);
void			paint_search(t_search *search, t_select *select);
void			paint_search_input(t_search *search, t_select *select);

void			print_header(t_select *select);

int				putchar_tty(int c);
int				putf_tty(const char *format, ...);

t_winsize		get_terminal_size(void);

bool			instanciate_items(t_selector *selector, int count, char **texts);
size_t			calculate_max_text_len_items(t_item *items, size_t len);
bool			modify_items(t_select *select);
bool			print_items(t_item *items, size_t len);
t_item			*item_from_id(t_item *items, size_t len, int id);

void			move_cursor(t_select *select, int index, int move);

void			pad(size_t pad_of);
bool			paint(t_item *items, t_select *select, t_search *search);
bool			loop(int count, char **items);

extern bool		g_resize;

#endif
