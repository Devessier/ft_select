/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/18 10:49:16 by bdevessi          #+#    #+#             */
/*   Updated: 2019/03/18 14:45:23 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <termcap.h>
#include <stdlib.h>
#include "select.h"

void		setup_sig_handlers(void)
{
	signal(SIGABRT, signal_handler);
	signal(SIGINT, signal_handler);
	signal(SIGSTOP, signal_handler);
	signal(SIGKILL, signal_handler);
	signal(SIGQUIT, signal_handler);
	signal(SIGWINCH, signal_handler);
	signal(SIGTSTP, signal_handler);
	signal(SIGCONT, signal_handler);
}

static void	reset_for_exit(void)
{
	char	*termcap;

	setup_termios(true);
	if ((termcap = tgetstr("te", NULL)) != NULL)
		tputs(termcap, 1, putchar_tty);
	if ((termcap = tgetstr("ve", NULL)) != NULL)
		tputs(termcap, 1, putchar_tty);
	if ((termcap = tgetstr("cl", NULL)) != NULL)
		tputs(termcap, 1, putchar_tty);
}

void		signal_handler(int sig)
{
	char	*termcap;

	if (sig == SIGTSTP)
	{
		reset_for_exit();
		signal(SIGTSTP, SIG_DFL);
		ioctl(fd(), TIOCSTI, "\x1A");
	}
	else if (sig == SIGCONT)
	{
		setup_termios(false);
		g_resize = true;
		if ((termcap = tgetstr("ti", NULL)) != NULL)
			tputs(termcap, 1, putchar_tty);
		if ((termcap = tgetstr("cl", NULL)) != NULL)
			tputs(termcap, 1, putchar_tty);
		if ((termcap = tgetstr("vi", NULL)) != NULL)
			tputs(termcap, 1, putchar_tty);
		setup_sig_handlers();
	}
	else if (sig == SIGWINCH)
	{
		if ((termcap = tgetstr("cl", NULL)) != NULL)
			tputs(termcap, 1, putchar_tty);
		g_resize = true;
	}
	else
	{
		reset_for_exit();
		exit(1);
	}
}
