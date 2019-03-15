/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/14 11:33:02 by bdevessi          #+#    #+#             */
/*   Updated: 2019/03/15 15:07:55 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <fcntl.h>
#include "libft.h"

int	fd(void)
{
	static int	fd = -1;

	if (fd == -1)
		if ((fd = open("/dev/tty", O_RDWR)) == -1)
			return (-1);
	return (fd);
}

int	putchar_tty(int c)
{
	const int	_fd = fd();

	if (_fd == -1)
		return (-1);
	return (write(_fd, &c, 1));
}

int	putf_tty(const char *format, ...)
{
	const int	_fd = fd();
	va_list		args;

	if (_fd == -1)
		return (-1);
	va_start(args, format);
	ft_putf_va(_fd, format, args);
	va_end(args);
	return (1);
}
