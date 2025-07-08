NAME = ft_nm
MODE ?= release

BIN_DIR = .bin-$(MODE)
INCLUDES = -Iincludes -Ilibft/includes

CC = cc
CFLAGS = -Wall -Werror -Wextra -MD $(INCLUDES)

ifeq ($(MODE), debug)
	CFLAGS = -Wall -Wextra -MD $(INCLUDES) -g3
endif

VPATH = srcs

SRCS =	main.c	\
		utils.c	\
		parse.c	\
		run.c	\

OBJS = $(addprefix $(BIN_DIR)/, $(SRCS:.c=.o))
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
	$(MAKE) -C libft

debug:
	$(MAKE) clean
	$(MAKE) -C libft debug
	$(MAKE) MODE=debug all

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) libft/libft.a -o $@

$(BIN_DIR)/%.o: %.c Makefile | $(BIN_DIR)
	$(CC) $(CFLAGS) -c $< -o $@
	printf "$(GRAY)compiling: $(BLUE)%-40s $(GRAY)[%d/%d]\n" "$<" "$$(ls $(BIN_DIR) | grep -c '\.o')" "$(words $(SRCS))"

clean:
	$(MAKE) -C libft clean
	rm -rf .bin-*

fclean:
	$(MAKE) -C libft fclean
	rm -rf .bin-release .bin-debug
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re debug libft

-include $(DEPS)

.SILENT:
MAKEFLAGS=--no-print-directory
