NAME_CLIENT := client
NAME_SERVER := server

SRCS_CLIENT_DIR := ./srcs/client
SRCS_SERVER_DIR := ./srcs/server
OBJS_ROOT_DIR := ./objs
OBJS_CLIENT_DIR := ./objs/client
OBJS_SERVER_DIR := ./objs/server

SRCS_CLIENT := main.c \
			   utils.c \

SRCS_SERVER := main.c \
			   utils.c \

OBJS_CLIENT := $(SRCS_CLIENT:.c=.o)
OBJS_SERVER := $(SRCS_SERVER:.c=.o)

SRCS_CLIENT := $(addprefix $(SRCS_CLIENT_DIR)/, $(SRCS_CLIENT))
SRCS_SERVER := $(addprefix $(SRCS_SERVER_DIR)/, $(SRCS_SERVER))

OBJS_CLIENT := $(addprefix $(OBJS_CLIENT_DIR)/, $(OBJS_CLIENT))
OBJS_SERVER := $(addprefix $(OBJS_SERVER_DIR)/, $(OBJS_SERVER))

SRCS_BONUS_CLIENT_DIR := ./srcs/client_bonus
SRCS_BONUS_SERVER_DIR := ./srcs/server_bonus
OBJS_BONUS_CLIENT_DIR := ./objs/client_bonus
OBJS_BONUS_SERVER_DIR := ./objs/server_bonus

SRCS_BONUS_CLIENT := main_bonus.c \
			   utils_bonus.c \

SRCS_BONUS_SERVER := main_bonus.c \
			   utils_bonus.c \

OBJS_BONUS_CLIENT := $(SRCS_BONUS_CLIENT:.c=.o)
OBJS_BONUS_SERVER := $(SRCS_BONUS_SERVER:.c=.o)

SRCS_BONUS_CLIENT := $(addprefix $(SRCS_BONUS_CLIENT_DIR)/, $(SRCS_BONUS_CLIENT))
SRCS_BONUS_SERVER := $(addprefix $(SRCS_BONUS_SERVER_DIR)/, $(SRCS_BONUS_SERVER))

OBJS_BONUS_CLIENT := $(addprefix $(OBJS_BONUS_CLIENT_DIR)/, $(OBJS_BONUS_CLIENT))
OBJS_BONUS_SERVER := $(addprefix $(OBJS_BONUS_SERVER_DIR)/, $(OBJS_BONUS_SERVER))

INC_DIR := ./includes ./libft/includes
INCLUDES := $(addprefix -I, $(INC_DIR))

LIBFT_DIR := ./libft
LIBFT := libft.a

CC := cc
CFLAGS := -Wall -Wextra -Werror
MAKE := make
RM := rm -rf

ifeq ($(MAKECMDGOALS), bonus)
	OBJS_CLIENT = $(OBJS_BONUS_CLIENT)
	OBJS_SERVER = $(OBJS_BONUS_SERVER)
endif

.PHONY: all
all: $(NAME_CLIENT) $(NAME_SERVER)

$(OBJS_CLIENT_DIR)/%o:$(SRCS_CLIENT_DIR)/%c
	@if [ ! -e `dirname $@` ]; then mkdir -p `dirname $@`; fi
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

$(OBJS_SERVER_DIR)/%o:$(SRCS_SERVER_DIR)/%c
	@if [ ! -e `dirname $@` ]; then mkdir -p `dirname $@`; fi
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

$(OBJS_BONUS_CLIENT_DIR)/%o:$(SRCS_BONUS_CLIENT_DIR)/%c
	@if [ ! -e `dirname $@` ]; then mkdir -p `dirname $@`; fi
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

$(OBJS_BONUS_SERVER_DIR)/%o:$(SRCS_BONUS_SERVER_DIR)/%c
	@if [ ! -e `dirname $@` ]; then mkdir -p `dirname $@`; fi
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

$(NAME_CLIENT): $(LIBFT) $(OBJS_CLIENT)
	$(CC) $(CFLAGS) $(OBJS_CLIENT) $(LIBFT) -o $(NAME_CLIENT)

$(NAME_SERVER): $(LIBFT) $(OBJS_SERVER)
	$(CC) $(CFLAGS) $(OBJS_SERVER) $(LIBFT) -o $(NAME_SERVER)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)
	mv $(addprefix $(LIBFT_DIR)/, $(LIBFT)) ./

.PHONY: clean
clean:
	$(MAKE) clean -C $(LIBFT_DIR)
	$(RM) $(OBJS_ROOT_DIR)

.PHONY: fclean
fclean: clean
	$(MAKE) fclean -C $(LIBFT_DIR)
	$(RM) $(LIBFT)
	$(RM) $(NAME_CLIENT) $(NAME_SERVER)

.PHONY: re
re: fclean all

.PHONY: norm
norm:
	-@norminette includes/
	-@norminette srcs/

.PHONY: bonus
bonus: all
