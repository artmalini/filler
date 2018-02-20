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

void	free_map(t_vis *map)
{
	int 	i;

	i = -1;
	while (++i < map->hcard)
		ft_memdel((void **)&map->card[i]);
	ft_memdel((void **)&map->card);

		//free(map->hcard[i]);
	//free(map->hcard);
}


/*void	get_vis_player(char *output, t_vis *mp)
{
	int 	i;

	i = 22;
	//fprintf(stderr, "output %s\n", output);
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
	fprintf(stderr, "mp->p1 %s mp->p2 %s\n", mp->p1, mp->p2);
}*/

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
		fprintf(stderr, "mp %s\n", mp->card[i]);
		free(param);
	}
	mp->card[mp->hcard] = NULL;
	//fprintf(stderr, "i %d\n", i);
	if (i != mp->hcard)
		return (-1);
	return (1);	
}

/*int		get_vis_map(char *output, t_vis *mp)
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
	if (!build_vis_map(mp))
	{
		return (-1);
	}
	return (1);	
}
*/





int 	get_my_player(char *output, t_vis *mp)
{
	//fprintf(stderr, "%s\n", output);
	if (ft_strstr(output, "p1"))
		mp->p1 = ft_strdup("amakhiny");
	else if (ft_strstr(output, "p2"))
		mp->p2 = ft_strdup("amakhiny");
	else
		return (-1);
	return (1);
}

int 	get_enem_player(char *output, t_vis *mp)
{
	int 	i;
	i = 22;

	while (output[++i])
	{
		if (output[i] == '.')
			break ;
	}
	if (ft_strstr(output, "p1"))			
		mp->p1 = ft_strsub((const char*)output, 23, i - 23);
	else if (ft_strstr(output, "p2"))
		mp->p2 = ft_strsub((const char*)output, 23, i - 23);
	else
		return (-1);
	return (1);	
}

int		get_plr(t_vis *mp)
{
	char	*output;

	while (get_next_line(0, &output) > 0 && !ft_strstr(output, "Plateau"))
	{
		if (ft_strstr(output, " : [./amakhiny.filler]"))
		{
			if(!(get_my_player(output, mp)))
			{
				free(output);				
				return (-1);
			}
		}
		if (ft_strstr(output, " : [players/"))
		{
			if(!(get_enem_player(output, mp)))
			{
				free(output);				
				return (-1);
			}
		}		
		free(output);
	}
	return (1);
}

int 	get_plateau(t_vis *mp)
{
	char	*output;

	//if (!ft_strstr(output, "Plateau"))
	//	return (-1);	
	mp->hcard = ft_atoi(output + 8);
	mp->wcard = ft_atoi(output + (8 + ft_int_length(mp->hcard)));
	free(output);
	if (1 != get_next_line(0, &output))
	{
		free(output);
		return (-1);
	}
	if (!build_vis_map(mp))
	{
		free(output);
		return (-1);
	}
	return (1);
}

int 	get_other(t_vis *mp)
{
	//int 	i;
	int 	y;
	int 	x;
	int 	strlen;
	char	*output;

	//i = -1;
	//if (1 != get_next_line(0, &output))
	//	return (-1);
	//fprintf(stderr, "output %s\n", output);

	//if (!ft_strstr(output, "Piece"))
	//{
	//	free(output);
	//	return (-1);	
	//}	
	//y = ft_atoi(output + 6);
	//x = ft_atoi(output + (6 + ft_int_length(y)));
	//strlen = (size_t)x;
	//fprintf(stderr, "strlen %d\n", x);
	//free(output);
	while (get_next_line(0, &output) > 0)
	{
		//if(ft_strlen(output) != strlen)
		//	return (-1);
		fprintf(stderr, "output %s\n", output);
		free(output);
	}
	return (0);
}

int 	parse_param(t_vis *mp)
{
	char	*output;

	if (!get_plr(mp))
		return (-1);
	while(get_next_line(0, &output) > 0)
	{
		if (ft_strstr(output, "Plateau"))
		{		
			if(!(get_plateau(mp)))
				return (-1);
		}
		if (ft_strstr(output, "Piece"))
			get_other(mp);
		free(output);
	}
	//if (!get_plateau(mp))
	//	return (-1);
	//if (!get_tetr(mp))
	//	return (-1);
	return (1);
}

/*void	get_input(t_vis *mp)
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
		if (ft_strstr(output, "Piece"))
		{
			//free(output);
			return ;
		}
		free(output);
	}
}*/
















t_vis 	*color(t_vis *mp, int r, int g, int b)
{
	mp->r = r;
	mp->g = g;
	mp->b = b;
	return (mp);
}

/*void 	image_header(t_vis *mp)
{
	mp->drw = mlx_get_data_addr(mp->img, &(mp->bits_per_pixel), &(mp->size_line), &(mp->endian));
}*/

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
}

void		vis_square(int x, int y, int size, t_vis *mp)
{
	int		ix;
	int		iy;
	int		tmp;

	iy = 0;
	tmp = 0;
	while (iy < (size * 4))
	{
		ix = 0;
		while (ix < (size * 4))
		{
			mp->drw[ix + ((y * WIDTH * 4) + (x * 4)) + tmp] = mp->b;
			mp->drw[ix + 1 + ((y * WIDTH * 4) + (x * 4)) + tmp] = mp->g;
			mp->drw[ix + 2 + ((y * WIDTH * 4) + (x * 4)) + tmp] = mp->r;
			mp->drw[ix + 3 + ((y * WIDTH * 4) + (x * 4)) + tmp] = 0;
			ix += 4;
		}
		iy += 4;
		tmp += (WIDTH * 4);
	}
}


void 	vis_map(t_vis *mp)
{
	int 	y;
	int 	x;
	int 	nb;

	y = -1;
	x = -1;
	nb = ((WIDTH / 2) / (mp->wcard));
	while (++y < mp->hcard)
	{
		while (++x < mp->wcard)
		{
			if (mp->card[y][x] != 'O')
				color(mp, 55, 55, 55);
			else if (mp->card[y][x] == 'O')
				color(mp, 55, 255, 55);
			vis_square((x * nb) + (x * 2), (y * nb) + (y * 2) + ((600 - (mp->hcard * nb) - (mp->hcard * 2)) / 2), nb, mp);			
		}
		x = -1;
	}
}

void 	vis_menu(t_vis *mp)
{
	vis_square(0, 0, WIDTH / 2, color(mp, 211, 151, 53));
	vis_map(mp);
}


int 	image_builder(t_vis *mp)
{
	char 	*str;

	if (mp->stop == 0)
	{
		fprintf(stderr, "yep\n");
		mp->drw = mlx_get_data_addr(mp->img, &(mp->bits_per_pixel), &(mp->size_line), &(mp->endian));

		if(!parse_param(mp))
		{
			//mlx_destroy_image(mp->mlx, mp->img);
			return (-1);
		}
		fprintf(stderr, "mp->p1 %s mp->p2 %s\n", mp->p1, mp->p2);
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
	return (1);
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
	if(!(image_builder(mp)))
		return (-1);
	return (1);
}


int 	mouse_hook(int key, int x, int y, t_vis *mp)
{
	if (key == 1)
	{
		if (mp->stop == 0)
			mp->stop = 1;
		else if (mp->stop == 1)
			mp->stop = 0;
	}	
	if(!(image_builder(mp)))
		return (-1);
	return (1);
}

void	vis_mp(t_vis *mp)
{
	//mp->p1 = NULL;
	//mp->p2 = NULL;
	mp->card = NULL;
	mp->hcard = 0;
	mp->wcard = 0;
	mp->stop = 0;
	mp->r = 0;
	mp->g = 0;
	mp->b = 0;
}

int		main(void)
{
  t_vis	*mp;

  mp = (t_vis*)malloc(sizeof(t_vis));
  vis_mp(mp);
  mp->mlx = mlx_init();
  mp->win = mlx_new_window(mp->mlx, WIDTH, HEIGHT, "Filler");
  mp->img = mlx_new_image(mp->mlx, WIDTH, HEIGHT);

  mlx_hook(mp->win, 2, 1, vis_hook, mp);
  mlx_mouse_hook(mp->win, mouse_hook, mp);
  if(!image_builder(mp))
	return (-1);
  mlx_loop_hook(mp->mlx, image_builder, mp);//no event

  //fprintf(stderr, "mp->p1 %s mp->p2 %s\n", mp->p1, mp->p2);
  //fprintf(stderr, "mp->card[0] %s\n", mp->card[0]);

  free_map(mp);
  mlx_loop(mp->mlx);
  free(mp);
 // system("leaks vis");
  return (0);
}
