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

void ft_getz(t_vec3d *vec,char *z)
{
	char **split;

	split = ft_split(z, ',');
	vec->z = ft_atoi(split[0]);
	if (split[1])
		vec->colour = atoi(split[1]);
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
			map[(i * y) + j] = (t_vec3d *)malloc(sizeof(t_vec3d *));
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

void ft_drawline(t_img data, int x0, int y0, int x1, int y1)
{
	int dx, dy, p, x, y;
	dx=x1-x0;
	dy=y1-y0;
	x=x0;
	y=y0;
	p=2*dy-dx;
	while(x<x1)
	{
		if(p>=0)
		{
			ft_putpixel(&data,x,y,0x00FF0000);
			y=y+1;
			p=p+2*dy-2*dx;
		}
		else
		{
			ft_putpixel(&data,x,y,0x00FF0000);
			p=p+2*dy;}
		x=x+1;
	}
}

int toIsometric2D(int x, int y, int z, int *u, int *v){
	*u = (x-z)/sqrt(2);
	*v = (x+2*y+z)/sqrt(6);
	return 0;
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
	list = ft_parsefdf("test_maps/20-60.fdf");
	t_vec3d **map = ft_map(list);
	for (int i; i < 400; i++)
	{
		printf("x : %f y: %f z: %f\n", map[i]->x, map[i]->y, map[i]->z);
		int u = 0;
		int v = 0;
		toIsometric2D(map[i]->x, map[i]->y, map[i]->z, &u, &v);
		if (i < 399) {
			int u2 = 0;
			int v2 = 0;
			toIsometric2D(map[i + 1]->x, map[i + 1]->y, map[i + 1]->z, &u2, &v2);
			ft_drawline(img, 342, 678, 789, 123);
			//ft_drawline(img, (u * 10) + win_width/2, (v * 10) + win_height/2, (u2 * 10) + win_width/2, (v2 * 10) + win_height/2);
			printf("u : %i u2: %i v: %i v2: %i\n", (u * 10) + win_width/2, (v * 10) + win_height/2, (u2 * 10) + win_width/2, (v2 * 10) + win_height/2);
		}
		if (i < 380) {
			int u3 = 0;
			int v3 = 0;
			toIsometric2D(map[i + 20]->x, map[i + 20]->y, map[i + 20]->z, &u3, &v3);
			ft_drawline(img, (u * 5) + win_width/2, (v * 5) + win_height/2, (u3 * 5) + win_width/2, (v3 * 5) + win_height/2);
		}
		ft_putpixel(&img, (u * 5) + win_width/2, (v * 10) + win_height/2, 0x00FF0000);
		printf("u : %i v: %i\n", u, v);
	}
	mlx_put_image_to_window(mlx, mlx_win, img.img, 0, 0);
	mlx_loop(mlx);
	return (0);
}