# include "image_processing/image_processing.h"
# include "minilibx-linux/mlx.h"
# include <X11/X.h>
# include <X11/keysym.h>



typedef struct s_mlx
{
	char	*name;
	void	*mlx_win;
	void	*mlx_connect;
	int		width;
	int		height;
	t_img	*img1;
	t_img	*img2;

}	t_mlx;

int	close_handler(t_mlx *info)
{
	mlx_destroy_image(info->mlx_connect, info->img1->img_ptr);
	mlx_destroy_image(info->mlx_connect, info->img2->img_ptr);

	mlx_destroy_window(info->mlx_connect, info->mlx_win);
	mlx_destroy_display(info->mlx_connect);
	
	free(info->mlx_connect);
	free(info->img1);
	free(info->img2);
	exit(EXIT_SUCCESS);
	return (0);
}

int	key_handler(int keysym, t_mlx *info)
{
	//printf("%d\n", keysym);
	if (keysym == XK_Escape)
		close_handler(info);
	return (0);
}

void	init(t_mlx *info)
{
	info->height = 1080;
	info->width  = 1080;
	info->name = "Combined Images";
	
	info->mlx_connect = mlx_init();
	if (info->mlx_connect == NULL)
		exit(EXIT_FAILURE);
	info->mlx_win = mlx_new_window(info->mlx_connect, info->width, info->height, info->name);
	if (info->mlx_win == NULL)
	{
		mlx_destroy_display(info->mlx_connect);
		perror("malloc failure");
	}
	mlx_hook(info->mlx_win, KeyPress, KeyPressMask, key_handler, info);

}

int	main(void)
{
	t_mlx info;
	t_img combined;
	
	init(&info);

	//char	*relative_path1 = "testing_super_frac_5/mandelbrot_4_15_n.png";

	
	//info.img1.img_ptr = mlx_xpm_file_to_image(info.mlx_connect, relative_path, &(info.width), &(info.height));
 	info.img1 = import_png(info.mlx_connect, "testing_super_frac_5/chaos_1.png");
    if (!info.img1) {
        fprintf(stderr, "Error: Failed to load PNG image.\n");
        return 1;
    }
	info.img2 = import_png(info.mlx_connect, "testing_super_frac_5/buddha_35_5k_-20_times_2.8.png");
    if (!info.img2) {
        fprintf(stderr, "Error: Failed to load PNG image.\n");
        return 1;
    }
	combined = combine_images(info.img1, info.img2, info.width, info.height, info.mlx_connect);

	
	mlx_put_image_to_window(info.mlx_connect,
		info.mlx_win, combined.img_ptr, 0, 0);
	mlx_loop(info.mlx_connect);

}
//-Limage_processing -limage_processing -lm -Lminilibx-linux -lmlx_Linux -lX11 -lXext -lpng -o combine
/*    void *mlx_ptr = mlx_init();
    void *win_ptr = mlx_new_window(mlx_ptr, 1080, 1080, "MiniLibX PNG Example");

    // Load PNG image
    t_img *image = import_png(mlx_ptr, "testing_super_frac_5/mandelbrot_6_15_n.png");
    if (!image) {
        fprintf(stderr, "Error: Failed to load PNG image.\n");
        return 1;
    }

    // Display image
    mlx_put_image_to_window(mlx_ptr, win_ptr, image->img_ptr, 0, 0);

    mlx_loop(mlx_ptr); */