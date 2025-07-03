NAME = ft_nm
MODE ?= release

OBJ_DIR = .bin-$(MODE)
INCLUDES = -Iincludes -Ilibft/includes

CC = cc
CFLAGS = -Wall -Werror -Wextra -MD $(INCLUDES) -g3

ifeq ($(MODE), debug)
	CC = cc
	CFLAGS = -Wall -Wextra -MD $(INCLUDES) -g3
endif

VPATH = srcs

SRCS =	main.c	\

OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))
DEPS = $(OBJS:.o=.d)
BIN = $(NAME)

RESET			= \033[0m
GRAY			= \033[90m
RED 			= \033[31m
GREEN 			= \033[32m
YELLOW 			= \033[33m
BLUE 			= \033[34m

all:
	$(MAKE) libft
	$(MAKE) $(BIN)
	printf "$(RESET)"

libft:
	printf "Compiling libft...\n"
	$(MAKE) -C libft > /dev/null

debug:
	$(MAKE) MODE=debug all

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) libft/libft.a -o $@

$(OBJ_DIR)/%.o: %.c Makefile |  $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@
	printf "$(GRAY)compiling: $(BLUE)%-40s $(GRAY)[%d/%d]\n" "$<" "$$(ls $(OBJ_DIR) | grep -c '\.o')" "$(words $(SRCS))"

clean:
	$(MAKE) -C libft clean
	rm -rf .bin-*

fclean:
	$(MAKE) -C libft fclean
	rm -rf .bin-*
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re debug libft

-include $(DEPS)

.SILENT:
MAKEFLAGS=--no-print-directory
