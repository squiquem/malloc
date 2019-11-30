# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: squiquem <squiquem@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/12/23 19:01:27 by squiquem          #+#    #+#              #
#    Updated: 2019/11/25 03:29:42 by squiquem         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

.PHONY			:	all clean fclean re opti

NAME			=	libft_malloc_$(HOSTTYPE).so

LINK			=	libft_malloc.so

SRC_DIR			=	./sources/

OBJ_DIR			=	./objects/

CPPFLAGS 		=	-I includes/

SRC_FILES		=	malloc.c \
					realloc.c \
					free.c \
					show_alloc_mem.c \
					block.c \
					page.c \
					utils.c \
					search.c \
					error.c

OBJ_FILES		=	$(SRC_FILES:.c=.o)

OBJ_D_FILES		=	$(SRC_FILES:.c=.d)

SRC				=	$(addprefix $(SRC_DIR)/,$(SRC_FILES))

OBJ				=	$(addprefix $(OBJ_DIR), $(OBJ_FILES))

OBJ_D			=	$(addprefix $(OBJ_DIR), $(OBJ_D_FILES))

LFT				=	-L libft/ -lft

CC				=	gcc

CFLAGS			=	-Wall -Wextra -Werror -MMD -O3

opti			:
	@make -j8 all

all				:
	@make all -C libft/
	@make $(NAME)

$(NAME)			:	$(OBJ)
	@$(CC) $(CFLAGS) $(LFT) $(OBJ) -o $@ -shared
	@rm -f $(LINK)
	@ln -s $(NAME) $(LINK)
	@printf '\033[4m'
	@printf "\033[32m[ ✔ ] $(NAME)\n\033[0m"

$(OBJ_DIR)%.o	:	$(SRC_DIR)%.c
	@mkdir $(OBJ_DIR) 2> /dev/null || true
	@$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ -c $<
	@printf '\033[0m[ ✔ ] %s\n\033[0m' "$<"

clean			:
	@make fclean -C libft/
	@rm -f $(OBJ)
	@rm -f $(OBJ_D)
	@rm -rf $(OBJ_DIR) 2> /dev/null || true
	@printf '\033[4m'
	@echo "\033[31mlibft_malloc.so:\033[0m"
	@printf '\033[31m[ ✔ ] %s\n\033[0m' "Clean Object Files"

fclean			:	clean
	@rm -f $(NAME) $(LINK)
	@printf '\033[31m[ ✔ ] %s\n\033[0m' "Clean Binary File"

re				:
	@make fclean
	@make opti

-include $(OBJ_D)
