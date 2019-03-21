/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/14 11:33:02 by bdevessi          #+#    #+#             */
/*   Updated: 2019/03/21 16:40:22 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
#include "libft.h"

int		fd(void)
{
	static int	fd = -1;

	if (fd == -1)
		if ((fd = open("/dev/tty", O_RDWR)) == -1)
			return (-1);
	return (fd);
}

int		putchar_tty(int c)
{
	const int	fdd = fd();

	if (fdd == -1)
		return (-1);
	return (write(fdd, &c, 1));
}

int		putf_tty(const char *format, ...)
{
	const int	fdd = fd();
	va_list		args;

	if (fdd == -1)
		return (-1);
	va_start(args, format);
	ft_putf_va(fdd, format, args);
	va_end(args);
	return (1);
}

bool	match(const char *s1, const char *s2)
{
	if (*s1 == *s2 && *s1 != '\0')
		return (match(s1 + 1, s2 + 1));
	if (*s2 == '*' && *s1 != '\0')
		return (match(s1 + 1, s2) || match(s1, s2 + 1));
	if (*s2 == '*' && *s1 == '\0')
		return (match(s1, s2 + 1));
	if (*s2 == '\0' && *s1 == '\0')
		return (true);
	return (false);
}
