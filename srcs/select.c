/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   select.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 11:22:41 by bdevessi          #+#    #+#             */
/*   Updated: 2019/03/14 11:37:32 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <termcap.h>
#include <termios.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
#include <term.h>
#include <fcntl.h>
#include "select.h"
#include "libft.h"

static void	setup_termios(bool reset)
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

void		signal_handler(int sig)
{
	if (sig == SIGWINCH)
		g_resize = true;
}

int		main(int argc, char **argv)
{
	const char	*term = getenv("TERM");

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
		signal(SIGWINCH, signal_handler);
		loop(--argc, ++argv);
		setup_termios(true);
	}
	return (0);
}
