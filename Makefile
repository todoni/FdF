
INCLUDES_DIR = ./includes/
LIBFT_INC_DIR = ./libft/
SOURCE_DIR = ./sources/
OBJECT_DIR = objects
FILES = \
		errors \
		graph \
		map \
		main \
		bresenham \
		color \
		init \
		setting \
		utils \
		utils2 \
		draw \

vpath %.c $(SOURCE_DIR)

SOURCES = $(addprefix $(SOURCE_DIR), $(addsuffix .c, $(FILES)))
OBJECTS = $(addprefix $(OBJECT_DIR)/, $(addsuffix .o, $(FILES)))

CC = gcc
CFLAGS = -Wall -Wextra -Werror
DLIBS	= -framework Metal -framework AppKit
LDFLAGS = -lmlx -Lmlx -Llibft -lft

NAME = fdf
LIBFT	= $(addprefix $(LIBFT_INC_DIR), libft.a)
MLX	= libmlx.dylib

all: $(NAME) 

$(NAME): $(OBJECT_DIR) $(OBJECTS) $(LIBFT) $(MLX)
	$(CC) $(CFLAGS) $(DLIBS) $(LDFLAGS) -I$(INCLUDES_DIR) $(OBJECTS) -o $@ 

$(OBJECT_DIR):
	@mkdir -p $(OBJECT_DIR)

$(OBJECT_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -I$(INCLUDES_DIR) -I$(LIBFT_INC_DIR) -c $< -o $@

$(LIBFT):
	@echo "make libft"
	@make -C libft/

$(MLX):
	@echo "make mlx"
	@make -C mlx/
	@cp mlx/$@ .

clean:
	rm -rf $(OBJECT_DIR)
	@make -C mlx/ clean
	@make -C libft/ clean

fclean: clean
	rm -f $(NAME) $(LIBFT) $(MLX)
	@make -C libft/ fclean

re : fclean all

.PHONY: all clean fclean re
