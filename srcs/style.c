/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   style.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/21 15:24:23 by bdevessi          #+#    #+#             */
/*   Updated: 2019/03/21 15:30:11 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "select.h"

void	text_align(t_item *item, t_box *box)
{
	const int	unused_space = box->width - item->text_len;

	if (item->text_align == RIGHT)
		pad(unused_space);
	else if (item->text_align == CENTER)
		pad(unused_space / 2);
}

void	color_item(t_item *item)
{
	char	*color;

	color = NULL;
	if (S_ISFIFO(item->file_type))
		color = COLOR_FIFO;
	else if (S_ISCHR(item->file_type))
		color = COLOR_CHR;
	else if (S_ISBLK(item->file_type))
		color = COLOR_BLK;
	else if (S_ISDIR(item->file_type))
		color = COLOR_DIR;
	else if (S_ISLNK(item->file_type))
		color = COLOR_LNK;
	else if (S_ISSOCK(item->file_type))
		color = COLOR_SOCK;
	else if (S_ISREG(item->file_type))
		color = GREEN_COLOR;
	if (color != NULL)
		putf_tty("%s%s" CSI "0m", color, item->text);
	else
		putf_tty("%s", item->text);
}
