#include "mlx/mlx.h"
#include "utils/utils.h"
#include <math.h>
#include <fcntl.h>

typedef struct s_img {
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}				t_img;

typedef struct	s_vec3d
{
	double x, y, z;
	int colour;
}				t_vec3d;

int win_width = 1920;
int win_height = 1080;

void ft_putpixel(t_img *data, int x, int y, int colour)
{
	char *dst;
	if(x > 0 && x < win_width && y > 0 && y < win_height)
	{
		dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8 ));
		*(unsigned int*)dst = colour;
	}
}

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
	if (list->line[count-1][0] == '\n')
		count--;
	return (count);
}

void ft_getz(t_vec3d *vec, char *z)
{
	char **split;

	split = ft_split(z, ',');
	vec->z = ft_atoi(split[0]);
	if (split[1])
		vec->colour = ft_atoi(split[1]);
	else
		vec->colour = 0;
}

t_vec3d **ft_map(t_list *list)
{
	t_vec3d **map;
	int x = ft_getx(list);
	int y = ft_gety(list);
	int i = 0;
	int j = 0;
	map = (t_vec3d **)malloc(sizeof(t_vec3d *) * x * y);
	while (i < y)
	{
		j = 0;
		while (j < x)
		{
			map[(i * y) + j] = (t_vec3d *)malloc(sizeof(t_vec3d));
			map[(i * y) + j]->y = i + 1;
			map[(i * y) + j]->x = j + 1;
			ft_getz(map[(i * y) + j], list->line[j]);
			j++;
		}
		list = list->next;
		i++;
	}
	return map;
}

void ft_drawline(t_img data, int x0, int y0, int x1, int y1) {

	int dx = abs(x1-x0);
	int sx = x0<x1 ? 1 : -1;
	int dy = abs(y1-y0);
	int sy = y0<y1 ? 1 : -1;
	int err = (dx>dy ? dx : -dy)/2, e2;

	while(1)
	{
		ft_putpixel(&data, x0, y0, 0x00FF0000);
		if (x0==x1 && y0==y1)
			break;
		e2 = err;
		if (e2 >-dx)
		{
			err -= dy;
			x0 += sx;
		}
		if (e2 < dy)
		{
			err += dx;
			y0 += sy;
		}
	}
}

static void offsetscale(t_vec3d *v)
{
	v->x = (win_width / 2) + (v->x * 40);
	v->y = (win_height / 2) + (v->y * 40);
}

t_vec3d *toIsometric2D(t_vec3d *v)
{
	t_vec3d	*iso = (t_vec3d *)malloc(sizeof(t_vec3d));
	iso->x = (v->x - v->y) * cos(0.523599);
	iso->y = -v->z + (v->x + v->y) * sin(0.523599);
	iso->z = 0.0;
	iso->colour = v->colour;
	offsetscale(iso);
	printf("new: x: %f, y: %f\n", iso->x, iso->y);
	return (iso);
}

int	main(void)
{
	void	*mlx;
	void	*mlx_win;

	t_img	img;
    t_list *list;
	mlx = mlx_init();
	mlx_win = mlx_new_window(mlx, win_width, win_height, "test");
	img.img = mlx_new_image(mlx, win_width, win_height);
	img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length, &img.endian);
	list = ft_parsefdf("test_maps/10-2.fdf");
	t_vec3d **map = ft_map(list);
	for (int i = 0; i < 99; i++)
	{
		if (i < 99 && (i+1)%10 != 0) {
			t_vec3d *one= toIsometric2D(map[i]);
			t_vec3d *two= toIsometric2D(map[i+1]);
			ft_drawline(img, one->x, one->y, two->x, two->y);
		}
		if (i < 90) {
			t_vec3d *one= toIsometric2D(map[i]);
			t_vec3d *two= toIsometric2D(map[i+10]);
			ft_drawline(img, one->x, one->y, two->x, two->y);
		}
		printf("x: %f, y: %f, z: %f\n",map[0]->x, map[0]->y, map[0]->z);
		toIsometric2D(map[i]);
	}
	mlx_put_image_to_window(mlx, mlx_win, img.img, 0, 0);
	mlx_loop(mlx);
	return (0);
}