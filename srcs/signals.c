/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/18 10:49:16 by bdevessi          #+#    #+#             */
/*   Updated: 2019/03/22 15:53:33 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <termcap.h>
#include <stdlib.h>
#include "select.h"

void		setup_sig_handlers(void)
{
	signal(SIGTERM, signal_handler);
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, signal_handler);
	signal(SIGABRT, signal_handler);
	signal(SIGKILL, signal_handler);
	signal(SIGTSTP, signal_handler);
	signal(SIGCONT, signal_handler);
	signal(SIGTTIN, signal_handler);
	signal(SIGTTOU, signal_handler);
	signal(SIGWINCH, signal_handler);
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

static void	handle_sigcont(void)
{
	char	*termcap;

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

void		signal_handler(int sig)
{
	if (sig == SIGTSTP)
	{
		reset_for_exit();
		signal(SIGTSTP, SIG_DFL);
		ioctl(fd(), TIOCSTI, "\x1A");
	}
	else if (sig == SIGCONT)
		handle_sigcont();
	else if (sig == SIGWINCH)
		g_resize = true;
	else
	{
		reset_for_exit();
		free(g_items);
		exit(1);
	}
}
