#include "mlx/mlx.h"
#include "utils/utils.h"
#include <math.h>
#include <fcntl.h>

typedef struct	s_data {
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}				t_data;

typedef struct	s_vec3d 
{
	float x, y;
	char *z;
	int colour;
}				t_vec3d;

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

// void ft_MultiplyMatrixVector(t_vec3d i, t_vec3d o, t_mat4x4 m)
// {
// 	o.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + m.m[3][0];
// 	o.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + m.m[3][1];
// 	o.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + m.m[3][2];
// 	o.colour = i.colour;
// 	float w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + m.m[3][3];

// 	if (w != 0.0f)
// 	{
// 		o.x /= w;
// 		o.y /= w;
// 		o.z /= w;
// 	}
// }

typedef struct s_list{
	char **line;
	int y;
	struct s_list *next;
} t_list;

t_list	*ft_parsefdf(char *file)
{
	int	fd;
	t_list *list = NULL;
	t_list *curr = NULL;
	t_list *new = NULL;
	char *line;
	int count = 1;

	fd = open(file, O_RDONLY);
	while(1){
		line = get_next_line(fd,100);
		if (!line) break;
		new = malloc(sizeof(t_list));
		new->next=NULL;
		new->line = ft_split(line,' ');
		new->y = count;
		if (!list) list = new;
		else {
			curr = list;
			while (curr->next)
				curr = curr->next;
			curr->next = new;
		}
		count++;
	}
	return (list);
}

int ft_gety(t_list *list)
{
	int count;
	t_list *curr;

	count = 1;
	if (!list) return (0);
	curr = list;
	while (curr->next)
	{
		curr = curr->next;
		count++;
	}
	return (count);
}

int ft_getx(t_list *list)
{
	int count;
	
	count = 0;
	if (!list) return (0);
	while (list->line[count])
		count++;
	return (count);
}

t_vec3d **ft_map(t_list *list)
{
	t_vec3d **map;
	int x = ft_getx(list);
	int y = ft_gety(list);
	int i = 0;
	int j = 0;
	map = (t_vec3d **)malloc(sizeof(t_vec3d *) * x * y);
	printf("x, y, %i, %i \n", x, y);
	while (i < y)
	{
		j = 0;
		while (j < x)
		{
			map[(i * y) + j] = (t_vec3d *)malloc(sizeof(t_vec3d *));
			map[(i * y) + j]->y = i + 1;
			map[(i * y) + j]->x = j + 1;
			map[(i * y) + j]->z = list->line[j];
			j++;
		}
		list = list->next;
		i++;
	}
	return map;
}

int	main(void)
{
	// t_mat4x4 matProj;
	// float fWidth = 1920;
	// float fHeight = 1080;
	// float fNear = 0.1;
	// float fFar = 1000;
	// float fFov = 90;
	// float fAspectRatio = fHeight / fWidth;
	// float fFovRad = 1 / tan(fFov * .5 / 180 * 3.14159);
	// matProj.m[0][0] = fAspectRatio * fFovRad;
	// matProj.m[1][1] = fFovRad;
	// matProj.m[2][2] = fFar / (fFar - fNear);
	// matProj.m[3][2] = (-fFar * fNear) / (fFar - fNear);
	// matProj.m[2][3] = 1.0;
	// matProj.m[3][3] = 0.0;

	// void	*mlx;
	// void	*mlx_win;
	// t_data	img;

	// mlx = mlx_init();
	// mlx_win = mlx_new_window(mlx, 1920, 1080, "test");
	// img.img = mlx_new_image(mlx, 1920, 1080);
	// img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length, &img.endian);
	// for (int i  = 10; i < 100; i++)
	// 	ft_putpixel(&img, 5, i, 0x000000FF);
	// mlx_put_image_to_window(mlx, mlx_win, img.img, 0, 0);
	// mlx_loop(mlx);
	t_list *list;
	t_vec3d **map;

	list = ft_parsefdf("test_maps/20-60.fdf");
	map = ft_map(list);
	for (int i = 0; i < 400; i++)
		printf("%s, %f, %f \n", map[i]->z, map[i]->x, map[i]->y);
}