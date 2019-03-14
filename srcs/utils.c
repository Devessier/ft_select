/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/14 11:33:02 by bdevessi          #+#    #+#             */
/*   Updated: 2019/03/14 11:51:02 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <fcntl.h>
#include "libft.h"

int	putchar_tty(int c)
{
	int	fd;
	int	nbytes;

	if ((fd = open("/dev/tty", O_RDWR)) == -1)
		return (-1);
	nbytes = write(fd, &c, 1);
	close(fd);
	return (nbytes);
}

int	putf_tty(const char *format, ...)
{
	int		fd;
	va_list	args;

	if ((fd = open("/dev/tty", O_RDWR)) == -1)
		return (-1);
	va_start(args, format);
	ft_putf_va(fd, format, args);
	va_end(args);
	close(fd);
	return (1);
}
