#include "mlx/mlx.h"

typedef struct	s_data {
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}				t_data;

typedef struct	s_vec3d 
{
	float x, y, z;
}				t_vec3d;

typedef struct	s_triangle
{
	t_vec3d p[3];
}				t_triangle;

typedef struct	s_mat4x4
{
	float m[4][4];
}				t_mat4x4;

void ft_putpixel(t_data *data, int x, int y, int colour)
{
	char *dst;

	dst = data->addr + (y * data->line_length + (x * (data->bits_per_pixel / 8 )));
	*(unsigned int*)dst = colour;
}

int	main(void)
{
	void	*mlx;
	void	*mlx_win;
	t_data	img;

	mlx = mlx_init();
	mlx_win = mlx_new_window(mlx, 1920, 1080, "test");
	img.img = mlx_new_image(mlx, 1920, 1080);
	img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length, &img.endian);
	for (int i  = 10; i < 100; i++)
	{
		ft_putpixel(&img, 5, i, 0x00FF0000);
	}
	mlx_put_image_to_window(mlx, mlx_win, img.img, 0, 0);
	mlx_loop(mlx);
}
