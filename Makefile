
NAME := minirt

SRC_DIR = ./src
OBJ_DIR = ./obj
INC_DIR = ./includes

SRCS := ./src/main.c ./src/init.c ./src/minirt.c ./src/events.c \
./src/render.c ./src/threader.c ./src/clean_up.c \
./src/math_utils.c \
./src/parsing/split_file.c \
./src/parsing/split_file_utils.c \
./src/parsing/parse_rt.c \
./src/parsing/check_amb_cam_light.c \
./src/parsing/check_sp_pl_cy.c \
./src/parsing/check_line_utils.c \
./src/parsing/check_line_utils2.c \
./src/init_objects/sphere_list_utils.c \
./src/init_objects/plane_list_utils.c \
./src/init_objects/cylinder_list_utils.c \
./src/init_objects/get_line_vals.c \
./src/init_objects/set_amb_cam_lights.c \
./src/parsing/parse_rt_utils.c \
./src/trace_objects/trace_planes.c \
./src/trace_objects/trace_spheres.c \
./src/testing/print_all_objects.c \
./src/testing/print_utils.c \
./src/free_all_objects.c \
./src/color_utils.c \
./src/forge/forge_rt.c \
./src/forge/write_spheres.c \
./src/forge/write_planes.c \
./src/forge/write_cylinders.c \
./src/forge/get_rt_name.c \
./src/png/export_png.c \
./src/png/export_png_utils.c \
./src/png/import_png.c \
./src/png/import_png_utils.c \
./src/png/get_png_name.c \
./src/downsample.c \


CFLAGS :=  -Wall -Wextra -Werror -I$(INC_DIR) -g -fPIE -march=native
#-Iincludes
RM := rm -f

LIBFT_PATH = libft

LIBFT_ARCH = $(LIBFT_PATH)/libft.a

CC := cc

#OBS := $(SRCS:.c=.o)

OBS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

#IMG_PATH = image_processing
#IMG_ARCH = $(IMG_PATH)/libimage_processing.a

COLOR_RESET = \033[0m
COLOR_GREEN = \033[1;92m
COLOR_BLUE = \033[1;96m 

MAKEFLAGS += --no-print-directory

define print_colored
	@echo "$(COLOR_GREEN)$(1)$(COLOR_BLUE)$(2)$(COLOR_RESET)$(3)"
endef

.SILENT:

all:  $(LIBFT_ARCH) $(NAME)

$(NAME): $(OBS)
	$(CC) $(OBS)  -L$(LIBFT_PATH) -lft -lm -Lminilibx-linux -lmlx_Linux -lX11 -lXext -o $@ -lpng
	$(call print_colored, "[SUCCESS]", "./$(NAME)", "Ready")

#	$(CC) $(OBS)  -L$(LIBFT_PATH) -lft -L$(IMG_PATH) -limage_processing -lm -Lminilibx-linux -lmlx_Linux -lX11 -lXext -o $@ -lpng


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT_ARCH):
	$(MAKE) -C $(LIBFT_PATH)

#$(IMG_ARCH):
#	$(MAKE) -C $(IMG_PATH)

clean:
	$(MAKE) -C $(LIBFT_PATH) clean
	$(RM) $(OBS)

fclean: clean
	$(MAKE) -C $(LIBFT_PATH) clean
	$(RM) $(NAME)
	
re: fclean all

.PHONY: all clean fclean re chaos
