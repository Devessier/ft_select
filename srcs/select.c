/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   select.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 11:22:41 by bdevessi          #+#    #+#             */
/*   Updated: 2019/03/18 16:01:10 by bdevessi         ###   ########.fr       */
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
#include <term.h>

void	setup_termios(bool reset)
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

int		main(int argc, char **argv)
{
	const int	_fd = fd();
	const char	*term = getenv("TERM");
	const int	result = term != NULL && tgetent(NULL, term);

	if (!(isatty(0) && _fd != -1 && term != NULL && result == 1))
	{
		if (term == NULL || result != 1)
			ft_putf_fd(2, "ft_select: " "%s\n", term == NULL ? "TERM env var is undefined or null" : "Couldn't load the termcaps DB");
		else
			ft_putf_fd(2, "ft_select: " "%s\n", _fd == -1 ? "Couldn't open /dev/tty" : "Stdin is not a tty");
		return (1);
	}
	if (argc == 1)
		return (0);
	if (term == NULL)
		ft_putf("TERM variable not set, exit\n");
	else
	{
		if (tgetent(NULL, term) != 1)
		{
			ft_putf("error\n");
			return (0);
		}
		setup_termios(false);
		setup_sig_handlers();
		loop(--argc, ++argv);
		setup_termios(true);
	}
	if (_fd != -1)
		close(_fd);
	return (0);
}
