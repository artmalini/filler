#include "visual.h"

#include <stdio.h>
typedef struct 	s_viz
{
	char 		*p1;
	char 		*p2;
	char 		**card;	
	int 		hcard;
	int 		wcard;
	int 		r;
	int 		g;
	int 		b;
	int 		stop;
	char 		*drw;
	int 		endian;
	int			bits_per_pixel;
	int 		size_line;
	void 		*mlx;
	void 		*win;
	void		*img;
}				t_vis;

void	get_vis_player(char *output, t_vis *mp)
{
	int 	i;

	i = 22;
	if (ft_strstr(output, "amakhiny"))
	{
		if (ft_strstr(output, "p1"))
			mp->p1 = ft_strdup("amakhiny");
		else
			mp->p2 = ft_strdup("amakhiny");
	}
	else
	{
		if (ft_strstr(output, "players/"))
		{
			while (output[++i])
			{
				if (output[i] == '.')
					break ;
			}
		if (ft_strstr(output, "p1"))			
			mp->p1 = ft_strsub((const char*)output, 23, i - 23);
		else
			mp->p2 = ft_strsub((const char*)output, 23, i - 23);
		}
	}
}

int 	build_vis_map(t_vis *mp)
{
	int 	i;
	size_t 	strlen;
	char	*param;

	i = -1;
	strlen = (size_t)(mp->wcard + 4);
	//fprintf(stderr, "mp->wcard %d mp->hcard %d\n", mp->wcard, mp->hcard);
	if (!(mp->card = (char **)malloc(sizeof(char *) * (mp->hcard + 1))))
		return (-1);	
	while (++i < mp->hcard && get_next_line(0, &param) > 0)
	{
		//fprintf(stderr, "param %s\n", param);
		if(ft_strlen(param) != strlen)
			return (-1);
		if (!(mp->card[i] = ft_strdup(param + 4)))
		{
			//free_mp_len(mp, i);
			//free(param);
			return (-1);
		}
		//fprintf(stderr, "mp %s\n", mp->card[i]);
		free(param);
	}
	mp->card[mp->hcard] = NULL;
	//fprintf(stderr, "i %d\n", i);
	if (i != mp->hcard)
		return (-1);
	return (1);	
}

int		get_vis_map(char *output, t_vis *mp)
{
	char 	*param;

	mp->hcard = ft_atoi(output + 8);
	mp->wcard = ft_atoi(output + (8 + ft_int_length(mp->hcard)));
	//free(output);  free this output in main
	//fprintf(stderr, "output %s | mp->hcard %d mp->wcard %d\n", output, mp->hcard, mp->wcard);
	if (1 != get_next_line(0, &param))
	{
		free(param);
		return (-1);
	}
	free(param);
	if (build_vis_map(mp) == -1)
	{
		return (-1);
	}
	return (1);	
}

void	get_input(t_vis *mp)
{
	char	*output;
	int		i;

	i = 0;
	while (get_next_line(0, &output) > 0)
	{
		if (ft_strstr(output, "$$$"))
		{
			get_vis_player(output, mp);
		}
		if (ft_strstr(output, "Plateau"))
		{
			if(!get_vis_map(output, mp))
				return ;
		}
		/*if (ft_strstr(output, "Piece"))
		{
			free(output);
			return ;
		}*/
		free(output);
	}
}






void	vis_background(t_vis *mp)
{
	int		i;

	i = 0;
	while (i < (HEIGHT * WIDTH * 4))
	{
		mp->drw[i] = 255;
		mp->drw[i + 1] = 246;
		mp->drw[i + 2] = 143;
		mp->drw[i + 3] = 0;
		i += 4;
	}
	mp->drw[i] = 0;
}

void		ft_draw_square(int start_x, int start_y, int size, t_vis *mp)
{
	int		i;
	int		i2;
	int		tmp;

	i2 = 0;
	tmp = 0;
	while (i2 < (size * 4))
	{
		i = 0;
		while (i < (size * 4))
		{
			mp->drw[i + tmp + ((start_y * WIDTH * 4) + (start_x * 4))] = mp->b;
			mp->drw[i + tmp + 1 + ((start_y * WIDTH * 4) + (start_x * 4))] = mp->g;
			mp->drw[i + tmp + 2 + ((start_y * WIDTH * 4) + (start_x * 4))] = mp->r;
			mp->drw[i + tmp + 3 + ((start_y * WIDTH * 4) + (start_x * 4))] = 0;
			i += 4;
		}
		i2 += 4;
		tmp += (WIDTH * 4);
	}
}

void 	vis_menu(t_vis *mp)
{
	mp->r = 211;
	mp->g = 151;
	mp->b = 53;
	ft_draw_square(0, 0, WIDTH / 2, mp);
}


int 	image_builder(t_vis *mp)
{
	char 	*str;

	if (mp->stop == 0)
	{
		fprintf(stderr, "yep\n");
		mp->drw = mlx_get_data_addr(mp->img, &(mp->bits_per_pixel), &(mp->size_line), &(mp->endian));

		get_input(mp);
		vis_background(mp);
		vis_menu(mp);//////////////

		mlx_put_image_to_window(mp->mlx, mp->win, mp->img, 0, 0);
		mlx_destroy_image(mp->mlx, mp->img);
		mp->img = mlx_new_image(mp->mlx, WIDTH, HEIGHT);
	}
	if (mp->stop == 1)
	{
		str = "PAUSE";
		mlx_string_put(mp->mlx, mp->win, (WIDTH / 4), (HEIGHT / 2), 0x0FFFFFF, str);
	}
	return (0);
}

int 	vis_hook(int key, t_vis *mp)
{
	if (key == 53)
	{
		//fprintf(stderr, "mp->stop %d\n", mp->stop);
		free(mp);//NOT at ALL!!! should be all clear
		exit(1);
	}
	if (key == 49)
	{	
		//fprintf(stderr, "key %d mp->stop %d\n", key, mp->stop);
		if (mp->stop == 0)
			mp->stop = 1;
		else
			mp->stop = 0;
	}
	image_builder(mp);
	return (0);
}

void	vis_mp(t_vis *mp)
{
	mp->card = NULL;
	mp->hcard = 0;
	mp->wcard = 0;
	mp->stop = 0;
	mp->r = 0;
	mp->g = 0;
	mp->b = 0;
}

int main(void)
{
  t_vis	*mp;

  mp = (t_vis*)malloc(sizeof(t_vis));
  vis_mp(mp);
  mp->mlx = mlx_init();
  mp->win = mlx_new_window(mp->mlx, WIDTH, HEIGHT, "Filler");
  mp->img = mlx_new_image(mp->mlx, WIDTH, HEIGHT);
  mlx_hook(mp->win, 2, 1, vis_hook, mp);
  image_builder(mp);
  //mlx_loop_hook(mp->mlx, image_builder, mp);//no event

  fprintf(stderr, "mp->p1 %s mp->p2 %s\n", mp->p1, mp->p2);
  fprintf(stderr, "mp->card[0] %s\n", mp->card[0]);
  mlx_loop(mp->mlx);
  free(mp);
 // system("leaks vis");
  return (0);
}
