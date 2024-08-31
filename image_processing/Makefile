NAME := libimage_processing.a
	
SRCS := mean_convo_filter.c gaussian_convo_filter.c my_pixel_puts.c \
export_png.c export_png_utils.c sample.c create_img.c adjust_pixels_rgb.c \
import_png.c import_png_utils.c pad_borders.c sobel.c convolution_utils.c \
pad_mirror.c printing_utils.c pad_mirror_kernel.c matrix_utils.c \
set_color_wheel.c get_next_name.c libft_utils.c interpolate_two_colors.c \
color_source_wheel.c threading_tools.c non_local_means.c pad_mirror_circle.c \


CFLAGS := -Wall -Wextra -Werror -fPIE

CC := cc

OBS := $(SRCS:.c=.o)

COLOR_RESET = \033[0m
COLOR_GREEN = \033[1;92m
COLOR_BLUE = \033[1;96m 

MAKEFLAGS += --no-print-directory


define print_colored
	@echo "$(COLOR_GREEN)$(1)$(COLOR_BLUE)$(2)$(COLOR_RESET)$(3)"
endef

.SILENT:

all: $(NAME)

$(NAME): $(OBS)
	ar rcs $(NAME) $(OBS)
	$(call print_colored, "[SUCCESS]", "archive $(NAME)", "Ready")

clean:
	rm -rf $(OBS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re