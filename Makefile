NAME := miniRT

SRC_DIR = ./src

OBJ_DIR = ./obj

INC_DIR = ./includes

SRCS := ./src/main.c \
./src/render/init.c \
./src/render/init_controls.c \
./src/render/minirt.c \
./src/render/minirt_super.c \
./src/render/minirt_low.c \
./src/render/render.c \
./src/render/set_transforms.c \
./src/render/set_transforms2.c \
./src/render/threader.c \
./src/render/utils.c \
./src/render/clean_up.c \
./src/render/free_all_objects.c \
./src/render/free_all_objects2.c \
./src/math_utils/vector_ops.c \
./src/math_utils/vector_ops2.c \
./src/math_utils/vector_ops3.c \
./src/math_utils/matrix/inverse.c \
./src/math_utils/matrix/inverse_utils.c \
./src/math_utils/matrix/multiply.c \
./src/math_utils/matrix/rotation.c \
./src/math_utils/matrix/print.c \
./src/math_utils/matrix/scale_tran_shear.c \
./src/math_utils/rotate_to.c \
./src/parsing/split_file.c \
./src/parsing/split_by_whitespace.c \
./src/parsing/split_file_utils.c \
./src/parsing/parse_rt.c \
./src/parsing/parse_rt_utils.c \
./src/parsing/handle_ids.c \
./src/parsing/check_amb_cam_light.c \
./src/parsing/check_sl_al.c \
./src/parsing/check_cu_hy.c \
./src/parsing/check_tx_tri.c \
./src/parsing/check_sp_pl_cy.c \
./src/parsing/check_line_utils.c \
./src/parsing/check_line_utils2.c \
./src/init_objects/sphere_list_utils.c \
./src/init_objects/push_pop/push_pop_sp.c \
./src/init_objects/push_pop/push_pop_pl.c \
./src/init_objects/push_pop/push_pop_cy.c \
./src/init_objects/push_pop/push_pop_cu.c \
./src/init_objects/push_pop/push_pop_cu_2.c \
./src/init_objects/push_pop/push_pop_hy.c \
./src/init_objects/push_pop/pop_lt.c \
./src/init_objects/push_pop/push_lt.c \
./src/init_objects/push_pop/push_lt_area_default.c \
./src/init_objects/push_pop/push_lt_area_default_2.c \
./src/init_objects/push_pop/push_lt_spot_default.c \
./src/init_objects/plane_list_utils.c \
./src/init_objects/cylinder_list_utils.c \
./src/init_objects/hyperboloid_list_utils.c \
./src/init_objects/cube_list_utils.c \
./src/init_objects/light_list_utils.c \
./src/init_objects/init_area_light.c \
./src/init_objects/tx_list_utils.c \
./src/init_objects/triangle_list_utils.c \
./src/init_objects/get_line_vals.c \
./src/init_objects/set_amb_cam_lights.c \
./src/init_objects/init_cam.c \
./src/trace_objects/intersection/trace_planes.c \
./src/trace_objects/intersection/trace_spheres.c \
./src/trace_objects/intersection/trace_cylinders.c \
./src/trace_objects/intersection/trace_hyperboloids.c \
./src/trace_objects/intersection/trace_cubes.c \
./src/trace_objects/shadow/shadow_rays.c \
./src/trace_objects/shadow/cyl_shadow.c \
./src/trace_objects/shadow/hyp_shadow.c \
./src/trace_objects/shadow/sp_shadow.c \
./src/trace_objects/shadow/pl_shadow.c \
./src/trace_objects/color_spheres.c \
./src/trace_objects/color_cylinders.c \
./src/trace_objects/color_hyperboloids.c \
./src/trace_objects/color_cubes.c \
./src/trace_objects/light_utils.c \
./src/trace_objects/refl_refr.c \
./src/trace_objects/set_indicies.c \
./src/testing/print_all_objects.c \
./src/testing/print_all_objects2.c \
./src/testing/print_utils.c \
./src/testing/cast_single_ray.c \
./src/testing/thread_testing.c \
./src/color_utils/build_textures.c \
./src/color_utils/color_utils.c \
./src/color_utils/color_utils2.c \
./src/color_utils/color_wheel.c \
./src/color_utils/color_disrupt.c \
./src/color_utils/texture_utils.c \
./src/color_utils/texture_utils2.c \
./src/color_utils/materials.c \
./src/color_utils/mapping.c \
./src/color_utils/map_cube.c \
./src/color_utils/map_cube2.c \
./src/color_utils/sine_cube.c \
./src/color_utils/bump_mapping.c \
./src/color_utils/build_lumin_map.c \
./src/color_utils/cyl_bump.c \
./src/color_utils/hyp_bump.c \
./src/color_utils/pl_bump.c \
./src/color_utils/cu_bump.c \
./src/color_utils/normal_patterns.c \
./src/events/main_events.c \
./src/events/print_guide.c \
./src/events/toggle_events.c \
./src/events/set_tx.c \
./src/events/set_on.c \
./src/events/prev_list_ob.c \
./src/events/next_list_ob.c \
./src/events/switch_list.c \
./src/events/push_pop.c \
./src/events/translation.c \
./src/events/rotation.c \
./src/events/rotate_colors.c \
./src/events/scaling.c \
./src/events/supersample.c \
./src/events/extra_events.c \
./src/events/mouse_events.c \
./src/events/mouse_move.c \
./src/forge/forge_rt.c \
./src/forge/write_spheres.c \
./src/forge/write_planes.c \
./src/forge/write_cylinders.c \
./src/forge/write_hyperboloids.c \
./src/forge/write_cubes.c \
./src/forge/write_textures.c \
./src/forge/write_lights.c \
./src/forge/write_splights.c \
./src/forge/write_arealights.c \
./src/forge/get_rt_name.c \
./src/png/export_png.c \
./src/png/export_png_utils.c \
./src/png/import_png.c \
./src/png/import_png_utils.c \
./src/png/import_png_utils2.c \
./src/png/get_png_name.c \
./src/bvh/groups.c \
./src/bvh/boxes.c \
./src/bvh/box_ops.c \
./src/bvh/box_ops2.c \
./src/events/menu/set_menu_vals.c \
./src/events/menu/set_light_vals.c \
./src/events/menu/set_lt_widlen.c \
./src/events/menu/dial_press.c \
./src/events/menu/resets.c \
./src/events/menu/quaternion.c \


CFLAGS :=  -Wall -Wextra -Werror -I$(INC_DIR) -g  -fPIE -Ofast -march=native -Wno-unused-result
#-mavx -mavx2, when ready to try some simd stuff

RM := rm -f

LIBFT_PATH = libft

LIBFT_ARCH = $(LIBFT_PATH)/libft.a

CC := cc

OBS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

COLOR_RESET = \033[0m
COLOR_GREEN = \033[1;92m
COLOR_BLUE = \033[1;96m 

MAKEFLAGS += --no-print-directory

define print_colored
	@echo "$(COLOR_GREEN)$(1)$(COLOR_BLUE)$(2)$(COLOR_RESET)$(3)"
endef

.SILENT:

all: $(LIBFT_ARCH) $(NAME)

$(NAME): $(OBS)
	$(CC) $(OBS)  -L$(LIBFT_PATH) -lft -lm -Lminilibx-linux -lmlx_Linux -lX11 -lXext -o $@ -lpng
	$(call print_colored, "[SUCCESS]", "./$(NAME)", "Ready")

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@


$(LIBFT_ARCH):
	$(MAKE) -C $(LIBFT_PATH)

#maybe change to not make the libft everytime
#libft:
#	$(MAKE) -C $(LIBFT_PATH)
#$(LIBFT_ARCH):

clean:
	$(MAKE) -C $(LIBFT_PATH) clean
	$(RM) $(OBS)

fclean: clean
	$(MAKE) -C $(LIBFT_PATH) fclean
	$(RM) $(NAME)
	
re: fclean all

.PHONY: all clean fclean re
