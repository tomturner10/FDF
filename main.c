#include "mlx/mlx.h"
#include "utils/utils.h"
#include <math.h>
#include <fcntl.h>


typedef struct	s_data {
	float fWidth;
	float fHeight;
	float fNear;
	float fFar;
	float fFov;
	float fAspectRatio;
	float fFovRad;
}				t_data;

typedef struct s_img {
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}				t_img;

typedef struct	s_vec3d 
{
	float x, y; 
	float z;
	int colour;
}				t_vec3d;

typedef struct	s_mat4x4
{
	float m[4][4];
}				t_mat4x4;

void ft_putpixel(t_img *data, int x, int y, int colour)
{
	char *dst;

	dst = data->addr + (y * data->line_length + (x * (data->bits_per_pixel / 8 )));
	*(unsigned int*)dst = colour;
}

void ft_MultiplyMatrixVector(t_vec3d *i, t_vec3d *o, t_mat4x4 *m)
{
	o->x = i->x * m->m[0][0] + i->y * m->m[1][0] + i->z * m->m[2][0] + m->m[3][0];
	o->y = i->x * m->m[0][1] + i->y * m->m[1][1] + i->z * m->m[2][1] + m->m[3][1];
	o->z = i->x * m->m[0][2] + i->y * m->m[1][2] + i->z * m->m[2][2] + m->m[3][2];
	o->colour = i->colour;
	float w = i->x * m->m[0][3] + i->y * m->m[1][3] + i->z * m->m[2][3] + m->m[3][3];

	if (w != 0.0f)
	{
		o->x /= w;
		o->y /= w;
		o->z /= w;
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
	printf("x, y, %i, %i \n", x, y);
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

t_data	*initdata()
{
	t_data *data;

	data = (t_data *)malloc(sizeof(t_data));
	data->fWidth = 1920;
	data->fHeight = 1080;
	data->fNear = 0.1;
	data->fFar = 1000;
	data->fFov = 90;
	data->fAspectRatio = data->fHeight / data->fWidth;
	data->fFovRad = 1 / tan(data->fFov * .5 / 180 * 3.14159);
	return (data);
}

t_mat4x4	*ft_initmat()
{
	t_mat4x4 *matProj;
	t_data *data = initdata();

	matProj = (t_mat4x4 *)malloc(sizeof(t_mat4x4));
	matProj->m[0][0] = data->fAspectRatio * data->fFovRad;
	matProj->m[1][1] = data->fFovRad;
	matProj->m[2][2] = data->fFar / (data->fFar - data->fNear);
	matProj->m[3][2] = (-data->fFar * data->fNear) / (data->fFar - data->fNear);
	matProj->m[2][3] = 1.0;
	matProj->m[3][3] = 0.0;
	return (matProj);
}

int	main(void)
{
	void	*mlx;
	void	*mlx_win;
	t_img	img;

	mlx = mlx_init();
	mlx_win = mlx_new_window(mlx, 1920, 1080, "test");
	img.img = mlx_new_image(mlx, 1920, 1080);
	img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length, &img.endian);
	t_list *list;
	t_vec3d **map;
	t_mat4x4 *mat;
	list = ft_parsefdf("test_maps/20-60.fdf");
	map = ft_map(list);
	mat = ft_initmat();
	t_vec3d *vec;
	vec = (t_vec3d *)malloc(sizeof(t_vec3d));
	for (int i = 0; i < 400; i++)
	{
		printf("%f, %f, %f, %i \n", map[i]->z, map[i]->x, map[i]->y, map[i]->colour);
		ft_MultiplyMatrixVector(map[i], vec, mat);
		printf("%f, %f, %f, %i \n", vec->z, vec->x, vec->y, vec->colour);
	}
	//mlx_put_image_to_window(mlx, mlx_win, img.img, 0, 0);
	//mlx_loop(mlx);
	return (0);
}