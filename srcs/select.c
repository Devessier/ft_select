/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   select.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 11:22:41 by bdevessi          #+#    #+#             */
/*   Updated: 2019/03/22 15:38:29 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <termcap.h>
#include <termios.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
#include <termcap.h>
#include <fcntl.h>
#include "select.h"
#include "libft.h"

void		setup_termios(bool reset)
{
	static struct termios	original_termios;
	struct termios			raw_termios_struct;

	if (reset)
		tcsetattr(0, TCSAFLUSH, &original_termios);
	else
	{
		tcgetattr(0, &original_termios);
		raw_termios_struct = original_termios;
		raw_termios_struct.c_iflag &= ~(ICRNL | IXON);
		raw_termios_struct.c_lflag &= ~(ECHO | ICANON | IEXTEN);
		raw_termios_struct.c_cc[VMIN] = 0;
		raw_termios_struct.c_cc[VTIME] = 1;
		tcsetattr(0, TCSAFLUSH, &raw_termios_struct);
	}
}

static int	handle_no_tty_no_term_no_getent(const char *term, const int result)
{
	if (!(isatty(0) && term != NULL && result == 1))
	{
		if (term == NULL || result != 1)
			ft_putf_fd(2, "ft_select: " "%s\n", term == NULL
			? "TERM env var is undefined or null"
			: "Couldn't load the termcaps DB");
		else
			ft_putf_fd(2, "ft_select: Stdin is not a tty\n");
		return (1);
	}
	return (0);
}

static int	print_no_args(void)
{
	ft_putf_fd(2, "ft_select: specify at least one parameter\n");
	return (0);
}

int			main(int argc, char **argv)
{
	const char	*term = getenv("TERM");
	const int	result = tgetent(NULL, term);
	int			ffd;

	if (argc == 1)
		return (print_no_args());
	if (handle_no_tty_no_term_no_getent(term, result) == 1)
		return (1);
	if (tgetent(NULL, term) != 1)
	{
		ft_putf_fd(2, "ft_select: Can't load %s terminfo entry\n", term);
		return (1);
	}
	if ((ffd = fd()) == -1)
	{
		ft_putf_fd(2, "ft_select: Couldn't open /dev/tty\n");
		return (1);
	}
	setup_termios(false);
	setup_sig_handlers();
	loop(--argc, ++argv);
	setup_termios(true);
	close(ffd);
	return (0);
}
