/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/14 16:37:55 by bdevessi          #+#    #+#             */
/*   Updated: 2019/03/21 23:29:37 by Devessier        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <term.h>
#include "select.h"
#include "libft.h"

void	print_header(t_select *select)
{
	if (!select->termcaps.mv_cursor)
		return ;
	tputs(tgoto(select->termcaps.mv_cursor, (select->window.ws_col - 15) / 2
		, 0), 1, putchar_tty);
	if (select->termcaps.maximum_colors > 0)
		putf_tty("\xF0\x9F\x90\xB1  " CSI "30;44m" " FT_SELECT " CSI "0m"
			" \xF0\x9F\x90\xB6");
	else
		putf_tty("\xF0\x9F\x90\xB1   FT_SELECT \xF0\x9F\x90\xB6");
}
