#include "visual.h"

#include <stdio.h>
/*
== O fin: 123
== X fin: 46
*/
typedef struct 	s_viz
{
	char 		*p1;
	char 		*p2;
	char 		**card;
	int 		p1_score;
	int 		p2_score;
	int 		hcard;
	int 		wcard;
	int 		r;
	int 		g;
	int 		b;
	int 		stop;
	int 		err;
	char 		*drw;
	int 		endian;
	int			bits_per_pixel;
	int 		size_line;
	void 		*mlx;
	void 		*win;
	void		*img;
}				t_vis;

void	free_map(t_vis *mp)
{
	int 	i;

	i = -1;
	while (++i < mp->hcard)
		ft_memdel((void **)&mp->card[i]);
	ft_memdel((void **)&mp->card);
}

void 	free_all_map(t_vis *mp)
{
	int 	i;

	i = -1;
	while (++i < mp->hcard)
		ft_memdel((void **)&mp->card[i]);
	ft_memdel((void **)&mp->card);

	if (mp->p1 != NULL)
		ft_strdel(&mp->p1);
	if (mp->p2 != NULL)
		ft_strdel(&mp->p2);
}

void	free_map_len(t_vis *mp, int len)
{
	int 	j;

	j = len - 1;
	while (0 <= --j && mp->card)
		ft_memdel((void **)&mp->card[len]);
	ft_memdel((void **)&mp->card);

	if (mp->p1 != NULL)
		ft_strdel(&mp->p1);
	if (mp->p2 != NULL)
		ft_strdel(&mp->p2);
}


void	get_vis_player(char *output, t_vis *mp)
{
	int 	i;

	i = 22;
	if (ft_strstr(output, "amakhiny"))
	{
		if (ft_strstr(output, "p1"))
			mp->p1 = ft_strdup("YOU");
		else
			mp->p2 = ft_strdup("YOU");
	}
	else
	{
		if (ft_strstr(output, "players/"))
		{
			while (++i && output[i] != '.');
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
		{
			free_map_len(mp, i);
			free(param);
			return (-1);
		}
		if (!(mp->card[i] = ft_strdup(param + 4)))
		{
			free_map_len(mp, i);
			free(param);
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
		return (-1);
	return (0);	
}

void 	find_error(t_vis *mp, char *output)
{
	if (ft_strstr(output, "error on input") || ft_strstr(output, "timedout"))
	{
		//fprintf(stderr, "@@@@output %s\n", output);
		mp->err = 0;
	}
	else
		mp->err = 1;
}

int		get_input(t_vis *mp)
{
	char	*output;

	while (get_next_line(0, &output) > 0)
	{
		if (ft_strstr(output, "$$$"))
			get_vis_player(output, mp);
		if (ft_strstr(output, "Plateau"))
			get_vis_map(output, mp);
		if (ft_strstr(output, "error"))
			find_error(mp, output);
		if (ft_strstr(output, "== O fin:"))
			mp->p1_score = ft_atoi(output + 9);
		if (ft_strstr(output, "== X fin:"))
			mp->p2_score = ft_atoi(output + 9);
		if (ft_strstr(output, "Piece"))
		{
			free(output);
			return (0);
		}
		//fprintf(stderr, "output %s\n", output);
		free(output);
	}
	return (0);
}








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
		mp->drw[i] = 175;
		mp->drw[i + 1] = 165;
		mp->drw[i + 2] = 158;
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
	//nb = ((WIDTH / 2) / (mp->wcard)) - 2;
	nb = (WIDTH / mp->wcard) - 2;
	while (++y < mp->hcard)
	{
		while (++x < mp->wcard)
		{
			if (mp->card[y][x] == '.')
				color(mp, 146, 152, 160);
			else if (mp->card[y][x] == 'X' || mp->card[y][x] == 'x')
				color(mp, 00, 00, 00);
			else if (mp->card[y][x] == 'O' || mp->card[y][x] == 'o')
				color(mp, 209, 12, 12);
			//vis_square((x * nb) + (x * 2) + 2, (y * nb) + (y * 2) + ((800 - (mp->hcard * nb) - (mp->hcard * 2)) / 2), nb, mp);
			vis_square((x * nb) + (x * 2) + 2, (y * nb) + (y * 2) + ((700 - (mp->hcard * nb) - (mp->hcard * 2)) / 2), nb, mp);	//15*45 = (675)	(30)	155	
		}
		x = -1;
	}
}

void 	vis_menu(t_vis *mp)
{
	//vis_square(0, 0, WIDTH / 2, color(mp, 211, 151, 53));
	if (mp->err == 0)
		vis_map(mp);
}

/*int		play_sound(char *command)
{
	int		status;

	status = 0;
	status = system(command);
	return (status);
}*/

void 	print_result(t_vis *mp)
{
	mlx_string_put(mp->mlx, mp->win, (WIDTH / 4) + 130, (HEIGHT / 2) - 60, 0x54dd58, mp->p1);
	if (mp->p1_score > mp->p2_score)
	{
		mlx_string_put(mp->mlx, mp->win, (WIDTH / 4) + 300, (HEIGHT / 2) - 60, 0x54dd58, "WIN!!!");
		//play_sound("afplay ./visual/sound/you-win.mp3 &");
	}
	else
		mlx_string_put(mp->mlx, mp->win, (WIDTH / 4) + 300, (HEIGHT / 2) - 60, 0x0000000, "Defeat");
	mlx_string_put(mp->mlx, mp->win, (WIDTH / 4) + 15, (HEIGHT / 2) - 25, 0x7c7c77, "Score:");	
	mlx_string_put(mp->mlx, mp->win, (WIDTH / 4) + 130, (HEIGHT / 2) - 25, 0x7c7c77, ft_itoa(mp->p1_score));


	mlx_string_put(mp->mlx, mp->win, (WIDTH / 4) + 130, (HEIGHT / 2) + 25, 0x0000000, mp->p2);
	if (mp->p2_score > mp->p1_score)
		mlx_string_put(mp->mlx, mp->win, (WIDTH / 4) + 300, (HEIGHT / 2) + 25, 0x54dd58, "WIN!!!");
	else
		mlx_string_put(mp->mlx, mp->win, (WIDTH / 4) + 300, (HEIGHT / 2) + 25, 0x0000000, "Defeat");
	mlx_string_put(mp->mlx, mp->win, (WIDTH / 4) + 15, (HEIGHT / 2) + 60, 0x7c7c77, "Score:");
	mlx_string_put(mp->mlx, mp->win, (WIDTH / 4) + 130, (HEIGHT / 2) + 60, 0x7c7c77, ft_itoa(mp->p2_score));
	mlx_string_put(mp->mlx, mp->win, (WIDTH / 4) + 15, (HEIGHT / 2) + 150, 0x0000000, "Press ESC to exit");	
	//fprintf(stderr, "ft_strlen(mp->p2) %zu\n", ft_strlen(mp->p2));
}

int 	image_builder(t_vis *mp)
{
	if (mp->stop == 0 && mp->err == 0)
	{
		mp->drw = mlx_get_data_addr(mp->img, &(mp->bits_per_pixel), &(mp->size_line), &(mp->endian));

		get_input(mp);
		vis_background(mp);
		vis_menu(mp);
		if (mp->p1_score != 0 || mp->p2_score != 0)
			vis_square(WIDTH / 4, (HEIGHT / 4), 400, color(mp, 255, 255, 255));

		mlx_put_image_to_window(mp->mlx, mp->win, mp->img, 0, 0);
		mlx_destroy_image(mp->mlx, mp->img);
		mp->img = mlx_new_image(mp->mlx, WIDTH, HEIGHT);
	}
	if (mp->stop == 1 && mp->err == 0 && (mp->p1_score == 0 || mp->p2_score == 0))
		mlx_string_put(mp->mlx, mp->win, (WIDTH / 2), (HEIGHT / 2), 0x0FFFFFF, "PAUSE");
	if (mp->err == 1)
		mlx_string_put(mp->mlx, mp->win, (WIDTH / 2), (HEIGHT / 2), 0x0FFFFFF, "ERROR");
	if (mp->p1_score != 0 || mp->p2_score != 0)
		print_result(mp);
	return (0);
}

int 	vis_hook(int key, t_vis *mp)
{
	if (key == 53)
	{
		//fprintf(stderr, "mp->stop %d\n", mp->stop);
		free_all_map(mp);//NOT at ALL!!! should be all clear
		free(mp);
		exit(1);
	}
	if (key == 49 && (mp->p1_score == 0 || mp->p2_score == 0))
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


int 	mouse_hook(int key, int x, int y, t_vis *mp)
{
	if (key == 1  && (mp->p1_score == 0 || mp->p2_score == 0))
	{
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
	mp->p1 = NULL;
	mp->p2 = NULL;
	mp->p1_score = 0;
	mp->p2_score = 0;
	mp->card = NULL;
	mp->hcard = 0;
	mp->wcard = 0;
	mp->stop = 0;
	mp->r = 0;
	mp->g = 0;
	mp->b = 0;
	mp->err = 0;
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
  image_builder(mp);
  mlx_loop_hook(mp->mlx, image_builder, mp);//no event

  //fprintf(stderr, "mp->p1 %s mp->p2 %s\n", mp->p1, mp->p2);
  //fprintf(stderr, "mp->card[0] %s\n", mp->card[0]);

  free_map(mp);
  mlx_loop(mp->mlx);
  free_all_map(mp);
  free(mp);
 // system("leaks vis");
  return (0);
}
