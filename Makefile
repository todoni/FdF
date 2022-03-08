
INCLUDE_DIR = ./includes/
SOURCE_DIR = ./sources/
OBJECT_DIR = objects
FILES = \
		errors \
		graph \
		map \
		main \

vpath %.c $(SOURCE_DIR)

SOURCES = $(addprefix $(SOURCE_DIR), $(addsuffix .c, $(FILES)))
OBJECTS = $(addprefix $(OBJECT_DIR)/, $(addsuffix .o, $(FILES)))

CC = gcc
CFLAGS = -Wall -Wextra -Werror -I$(INCLUDE_DIR) -Ilibft
DLIBS	= -framework Metal -framework AppKit

NAME = fdf
LIBFT	= libft.a
MLX	= libmlx.dylib

all: $(NAME) 

$(NAME): $(OBJECTS) $(LIBFT) $(MLX)
	$(CC) $(CFLAGS) $^ $(DLIBS) -I$(INCLUDE_DIR) -o $@

$(OBJECT_DIR):
	@mkdir -p $(OBJECT_DIR)

$(OBJECT_DIR)/%.o: %.c $(OBJECT_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
	@make bonus -C libft/
	@cp libft/$@ .

$(MLX):
	@make -C mlx/
	@cp mlx/$@ .

clean:
	rm -rf $(OBJECT_DIR)
	@make clean -C mlx/
	@make clean -C libft/

fclean: clean
	rm -f $(NAME) $(LIBFT) $(MLX)
	@rm -f $(LIBFT) -C libft/
	@rm -f $(MLX) -C mlx/

re : fclean all
