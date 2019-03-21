# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/03/11 10:38:02 by bdevessi          #+#    #+#              #
#    Updated: 2019/03/21 14:16:48 by bdevessi         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ft_select
CFLAGS = -Wall -Wextra -Werror -Iinclude -Ilibft/includes
CC = gcc

include src.mk

OBJS = $(SRCS:.c=.o)

all: $(NAME)

%.o: %.c Makefile src.mk include/select.h
	@ echo "Compiling \x1b[92m$*.c\x1b[0m..."
	@ $(CC) $(CFLAGS) -o $@ -c $<

$(NAME): $(OBJS)
	@ echo "Make libft..."
	@ $(MAKE) -C libft
	@ echo "Assemblating everything for \x1b[102;30m$(NAME)\x1b[0m..."
	@ $(CC) $(CFLAGS) -ltermcap -o $(NAME) $(OBJS) libft/libft.a
	@ echo "\x1b[102;30m$(NAME)\x1b[0m is done ! \xE2\x9C\x85"

clean:
	@ rm -f $(OBJS)
	@ echo "Cleaning \x1b[102;30m$(NAME)\x1b[0m done !"

fclean: clean
	@ echo "Super cleaning \x1b[102;30m$(NAME)\x1b[0m done !"

re: fclean $(NAME)

.PHONY: clean fclean re proto
