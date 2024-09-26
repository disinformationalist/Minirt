NAME := minirt

SRC_DIR = ./src
OBJ_DIR = ./obj
INC_DIR = ./includes

SRCS := ./src/main.c \
./src/init.c \
./src/minirt.c \
./src/render.c \
./src/set_transforms.c \
./src/clean_up.c \
./src/math_utils/vector_ops.c \
./src/math_utils/vector_ops2.c \
./src/math_utils/vector_ops3.c \
./src/math_utils/matrix_ops/matrix_2x2.c \
./src/math_utils/matrix_ops/matrix_3x3_init.c \
./src/math_utils/matrix_ops/matrix_3x3_ops.c \
./src/math_utils/matrix_ops/matrix_3x3_stack.c \
./src/math_utils/matrix_ops/matrix_3x3_transf.c \
./src/math_utils/matrix_ops/matrix_vec_conv.c \
./src/math_utils/matrix_ops/print_matrix.c \
./src/math_utils/matrix_ops/inverse.c \
./src/math_utils/rotate_to_up.c \
./src/parsing/split_file.c \
./src/parsing/split_by_whitespace.c \
./src/parsing/split_file_utils.c \
./src/parsing/parse_rt.c \
./src/parsing/check_amb_cam_light.c \
./src/parsing/check_sp_pl_cy.c \
./src/parsing/check_line_utils.c \
./src/parsing/check_line_utils2.c \
./src/init_objects/sphere_list_utils.c \
./src/init_objects/push_pop_sp.c \
./src/init_objects/plane_list_utils.c \
./src/init_objects/push_pop_pl.c \
./src/init_objects/cylinder_list_utils.c \
./src/init_objects/push_pop_cy.c \
./src/init_objects/get_line_vals.c \
./src/init_objects/set_amb_cam_lights.c \
./src/init_objects/init_cam.c \
./src/parsing/parse_rt_utils.c \
./src/trace_objects/trace_planes.c \
./src/trace_objects/trace_spheres.c \
./src/trace_objects/trace_capped_cylinders.c \
./src/trace_objects/shadow_rays.c \
./src/testing/print_all_objects.c \
./src/testing/print_utils.c \
./src/free_all_objects.c \
./src/color_utils.c \
./src/events/main_events.c \
./src/events/traverse_lists.c \
./src/events/transform_objects.c \
./src/events/extra_events.c \
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
./src/threader.c \
./src/color_disrupt.c \
./src/init_objects/light_list_utils.c \
#./src/trace_objects/trace_cylinders.c \


CFLAGS :=  -Wall -Wextra -Werror -I$(INC_DIR) -g -fPIE -Ofast -march=native -Wno-unused-result
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
#bonus:



clean:
	$(MAKE) -C $(LIBFT_PATH) clean
	$(RM) $(OBS)

fclean: clean
	$(MAKE) -C $(LIBFT_PATH) clean
	$(RM) $(NAME)
	
re: fclean all

.PHONY: all clean fclean re bonus
