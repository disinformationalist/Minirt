NAME := minirt
NAME_BONUS := minirt_bonus

SRC_DIR = ./src
SRC_DIR_BONUS = ./src_bonus
OBJ_DIR = ./obj
OBJ_DIR_BONUS = ./obj_bonus
INC_DIR = ./includes
INC_DIR_BONUS = ./includes_bonus

SRCS := ./src/main.c \
./src/init.c \
./src/minirt.c \
./src/minirt_super.c \
./src/render.c \
./src/set_transforms.c \
./src/clean_up.c \
./src/math_utils/vector_ops.c \
./src/math_utils/vector_ops2.c \
./src/math_utils/vector_ops3.c \
./src/math_utils/matrix_ops/matrix_vec_conv.c \
./src/math_utils/matrix_ops/print_matrix.c \
./src/math_utils/matrix_ops/inverse.c \
./src/math_utils/matrix_ops/inverse_utils.c \
./src/math_utils/matrix_ops/matrix_4x4_ops.c \
./src/math_utils/matrix_ops/matrix_rotation.c \
./src/math_utils/matrix_ops/matrix_scale_tran.c \
./src/math_utils/rotate_to.c \
./src/parsing/split_file.c \
./src/parsing/split_by_whitespace.c \
./src/parsing/split_file_utils.c \
./src/parsing/parse_rt.c \
./src/parsing/check_amb_cam_light.c \
./src/parsing/check_sp_pl_cy_le.c \
./src/parsing/check_line_utils.c \
./src/parsing/check_line_utils2.c \
./src/parsing/check_line_utils3.c \
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
./src/trace_objects/trace_spheres2.c \
./src/trace_objects/trace_capped_cylinders.c \
./src/trace_objects/trace_capped_cylinders2.c \
./src/trace_objects/trace_capped_cylinders3.c \
./src/trace_objects/shadow_rays.c \
./src/testing/print_all_objects.c \
./src/testing/print_all_objects2.c \
./src/testing/print_utils.c \
./src/free_all_objects.c \
./src/color_utils.c \
./src/events/main_events.c \
./src/events/traverse_lists.c \
./src/events/switch_list.c \
./src/events/push_pop.c \
./src/events/translation.c \
./src/events/rotation.c \
./src/events/scaling.c \
./src/events/supersample.c \

SRCS_B := ./src_bonus/main.c \
./src_bonus/init.c \
./src_bonus/minirt.c \
./src_bonus/minirt_super.c \
./src_bonus/render.c \
./src_bonus/set_transforms.c \
./src_bonus/clean_up.c \
./src_bonus/math_utils/vector_ops.c \
./src_bonus/math_utils/vector_ops2.c \
./src_bonus/math_utils/vector_ops3.c \
./src_bonus/math_utils/matrix_ops/matrix_vec_conv.c \
./src_bonus/math_utils/matrix_ops/print_matrix.c \
./src_bonus/math_utils/matrix_ops/inverse.c \
./src_bonus/math_utils/matrix_ops/inverse_utils.c \
./src_bonus/math_utils/matrix_ops/matrix_4x4_ops.c \
./src_bonus/math_utils/matrix_ops/matrix_rotation.c \
./src_bonus/math_utils/matrix_ops/matrix_scale_tran.c \
./src_bonus/math_utils/rotate_to.c \
./src_bonus/parsing/split_file.c \
./src_bonus/parsing/split_by_whitespace.c \
./src_bonus/parsing/split_file_utils.c \
./src_bonus/parsing/parse_rt.c \
./src_bonus/parsing/check_amb_cam_light.c \
./src_bonus/parsing/check_sp_pl_cy_le.c \
./src_bonus/parsing/check_line_utils.c \
./src_bonus/parsing/check_line_utils2.c \
./src_bonus/parsing/check_line_utils3.c \
./src_bonus/init_objects/sphere_list_utils.c \
./src_bonus/init_objects/push_pop_sp.c \
./src_bonus/init_objects/plane_list_utils.c \
./src_bonus/init_objects/push_pop_pl.c \
./src_bonus/init_objects/cylinder_list_utils.c \
./src_bonus/init_objects/push_pop_cy.c \
./src_bonus/init_objects/lens_list_utils.c \
./src_bonus/init_objects/push_pop_le.c \
./src_bonus/init_objects/light_list_utils.c \
./src_bonus/init_objects/push_pop_lt.c \
./src_bonus/init_objects/get_line_vals.c \
./src_bonus/init_objects/set_amb_cam_lights.c \
./src_bonus/init_objects/init_cam.c \
./src_bonus/parsing/parse_rt_utils.c \
./src_bonus/trace_objects/trace_planes.c \
./src_bonus/trace_objects/trace_spheres.c \
./src_bonus/trace_objects/color_spheres.c \
./src_bonus/trace_objects/trace_lenses.c \
./src_bonus/trace_objects/trace_capped_cylinders.c \
./src_bonus/trace_objects/color_cylinders.c \
./src_bonus/trace_objects/light_utils.c \
./src_bonus/trace_objects/shadow_rays.c \
./src_bonus/testing/print_all_objects.c \
./src_bonus/testing/print_utils.c \
./src_bonus/free_all_objects.c \
./src_bonus/color_utils.c \
./src_bonus/events/main_events.c \
./src_bonus/events/traverse_lists.c \
./src_bonus/events/switch_list.c \
./src_bonus/events/push_pop.c \
./src_bonus/events/translation.c \
./src_bonus/events/rotation.c \
./src_bonus/events/scaling.c \
./src_bonus/events/supersample.c \
./src_bonus/events/extra_events.c \
./src_bonus/forge/forge_rt.c \
./src_bonus/forge/write_spheres.c \
./src_bonus/forge/write_planes.c \
./src_bonus/forge/write_cylinders.c \
./src_bonus/forge/write_lights.c \
./src_bonus/forge/write_splights.c \
./src_bonus/forge/write_lenses.c \
./src_bonus/forge/get_rt_name.c \
./src_bonus/png/export_png.c \
./src_bonus/png/export_png_utils.c \
./src_bonus/png/import_png.c \
./src_bonus/png/import_png_utils.c \
./src_bonus/png/get_png_name.c \
./src_bonus/threader.c \
./src_bonus/color_disrupt.c \
./src_bonus/materials.c \


CFLAGS :=  -Wall -Wextra -Werror -I$(INC_DIR) -g -fPIE -Ofast -march=native -Wno-unused-result

CFLAGS_B :=  -Wall -Wextra -Werror -I$(INC_DIR_BONUS) -g -fPIE -Ofast -march=native -Wno-unused-result

RM := rm -f

LIBFT_PATH = libft

LIBFT_ARCH = $(LIBFT_PATH)/libft.a

CC := cc

OBS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

OBS_B = $(SRCS_B:$(SRC_DIR_BONUS)/%.c=$(OBJ_DIR_BONUS)/%.o)

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

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT_ARCH):
	$(MAKE) -C $(LIBFT_PATH)

bonus: $(LIBFT_ARCH) $(NAME_BONUS)

$(NAME_BONUS): $(OBS_B)
	$(CC) $(OBS_B)  -L$(LIBFT_PATH) -lft -lm -Lminilibx-linux -lmlx_Linux -lX11 -lXext -o $@ -lpng
	$(call print_colored, "[SUCCESS]", "./$(NAME_BONUS)", "Ready")

$(OBJ_DIR_BONUS)/%.o: $(SRC_DIR_BONUS)/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS_B) -c $< -o $@

clean:
	$(MAKE) -C $(LIBFT_PATH) clean
	$(RM) $(OBS) $(OBS_B)

fclean: clean
	$(MAKE) -C $(LIBFT_PATH) fclean
	$(RM) $(NAME) $(NAME_BONUS)
	
re: fclean all

.PHONY: all clean fclean re bonus
