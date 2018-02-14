
#include "libft/libft.h"
#include "includes/filler.h"

#include <stdio.h>

#include <time.h>
typedef struct  s_coords
{
	int			y;
	int			x;
}				t_place;

typedef struct	s_filler
{
	//int			p;
	int			hcard;
	int			wcard;
	int 		htetr;
	int 		wtetr;
	char		**card;
	char		**tetr;
	t_place		*coords;
}				t_filler;



void	free_map(t_filler *map)
{
	int 	i;

	i = -1;
	while (++i < map->hcard)
		ft_memdel((void **)&map->card[i]);
	ft_memdel((void **)&map->card);
	i = -1;
	while (++i < map->htetr)
		ft_memdel((void **)&map->tetr[i]);
	ft_memdel((void **)&map->tetr);
	ft_memdel((void **)&map->coords);

		//free(map->hcard[i]);
	//free(map->hcard);
}

void	free_map_len(t_filler *map, int len)
{
	int 	j;

	j = len - 1;
	while (0 <= --j && map->card)
		ft_memdel((void **)&map->card[len]);
	ft_memdel((void **)&map->card);
	
	while (0 <= --j && map->tetr)
		ft_memdel((void **)&map->tetr[len]);
	ft_memdel((void **)&map->tetr);

	ft_memdel((void **)&map->coords);
}



int		get_player(void)
{
	char	*output;

	if (1 != get_next_line(0, &output))
	{
		free(output);
		return (-1);
	}
	if (ft_strlen(output) < 18)
	{
		free(output);
		return (-1);
	}
	
	//fprintf(stderr, "plr str %s\n", plr);
	if (ft_strstr(output, "p1"))
	{
		free(output);
		return ('o');
	}
	else if (ft_strstr(output, "p2"))
	{
		free(output);
		return ('x');
	}
	else
	{
		free(output);
		return (-1);
	}
}

int 	build_map(t_filler *map)
{
	int 	i;
	size_t 	strlen;
	char	*output;

	i = -1;
	strlen = (size_t)(map->wcard + 4);
	//fprintf(stderr, "strlen %ju\n", strlen);
	if (!(map->card = (char **)malloc(sizeof(char *) * (map->hcard + 1))))
		return (-1);	
	while (++i < map->hcard && get_next_line(0, &output) > 0)
	{
		//if (1 != get_next_line(0, &output))
		//	return (-1);
		if(ft_strlen(output) != strlen)
			return (-1);
		if (!(map->card[i] = ft_strdup(output + 4)))
		{
			//free_map_len(map, i);
			//free(output);
			return (-1);
		}
		fprintf(stderr, "map %s\n", map->card[i]);
		free(output);
	}
	map->card[map->hcard] = NULL;
	//fprintf(stderr, "i %d\n", i);
	if (i != map->hcard)
		return (-1);
	return (1);
}

int		parse_map(t_filler *map)
{
	char	*param;
	//char	*output;

	int start = clock();

	if (1 != get_next_line(0, &param))
	{
		//fprintf(stderr, "param %s\n", output);
		return (-1);
	}
	//param = output;
	//fprintf(stderr, "param %s\n", output);
	//free(output);
	if (!ft_strstr(param, "Plateau"))
		return (-1);	
	map->hcard = ft_atoi(param + 8);
	map->wcard = ft_atoi(param + (8 + ft_int_length(map->hcard)));
	free(param);
	if (1 != get_next_line(0, &param))
		return (-1);
	free(param);
	if (build_map(map) == -1)
	{
		fprintf(stderr, "error build_map\n");
		return (-1);
	}
	//fprintf(stderr, "good\n");

	int end = clock();
	fprintf(stderr, "speed %f\n", ((float)end - start)/CLOCKS_PER_SEC);
	return (0);
}


int 	build_tetr(t_filler *map)
{
	int 	i;
	size_t 	strlen;
	char	*output;

	i = -1;
	strlen = (size_t)map->wtetr;
	//fprintf(stderr, "strlen %ju\n", strlen);
	if (!(map->tetr = (char**)malloc(sizeof(char*) * (map->htetr + 1))))
		return (-1);	
	while (++i < map->htetr && get_next_line(0, &output) == 1)
	{
		if(ft_strlen(output) != strlen)
			return (-1);
		if (!(map->tetr[i] = ft_strdup(output)))
		{
			//free_map_len(map, i);
			//free(output);
			return (-1);
		}
		fprintf(stderr, "tetr %s\n", map->tetr[i]);
		free(output);
	}
	map->tetr[map->htetr] = NULL;
	if (i != map->htetr)
		return (-1);
	return (1);
}
//map->coords.y
int		save_tetr(t_filler *map)
{
	int		i;
	int		j;
	int		z;

	i = -1;
	j = -1;
	z = 0;
	if (!(map->coords = (t_place*)malloc(sizeof(t_place) * (map->htetr * map->wtetr + 1))))
		return (-1);
	while (++i < map->htetr)
	{
		while (++j < map->wtetr)
		{
			if (map->tetr[i][j] == '*')
			{
				map->coords[z].x = j;
				map->coords[z++].y = i;
				fprintf(stderr, "tetr coords y %d x %d\n", j, i);
			}			
		}		
		j = -1;
	}
	map->coords[z].x = -1;
	return (1);
}

int		parse_tetr(t_filler *map)
{
	char	*output;

	if (1 != get_next_line(0, &output))
		return (-1);	
	if (!ft_strstr(output, "Piece"))
		return (-1);	
	map->htetr = ft_atoi(output + 6);
	map->wtetr = ft_atoi(output + (6 + ft_int_length(map->htetr)));
	free(output);
	if (build_tetr(map) == -1)
		return (-1);
	if (save_tetr(map) == -1)
		return (-1);
	return (0);
}










int 	try_fit_tetr(t_filler *map, t_place *try_fit, char plr)
{
	int 	i;
	int 	nbr;

	i = -1;
	nbr = 0;
	while (map->coords[++i].x != -1)
	{
		if (map->coords[i].y + try_fit->y >= map->hcard || map->coords[i].x + try_fit->x >= map->wcard)
			return (0);
		if (map->coords[i].y + try_fit->y < 0 || map->coords[i].x + try_fit->x < 0)
			return (0);
		else if (map->card[map->coords[i].y + try_fit->y][map->coords[i].x + try_fit->x] == plr - 32)
		{
			//fprintf(stderr, "found try_fit {plr %c} try_fit->y %d try_fit->x %d\n", plr - 32, try_fit->y, try_fit->x);
			nbr++;
		}
		else if (map->card[map->coords[i].y + try_fit->y][map->coords[i].x + try_fit->x] != '.')
			return (0);
	}
	return (nbr == 1 ? 1 : 0);
}

int 	minus_x(t_place *coords)
{
	int 	i;
	int 	x;

	i = -1;
	x = 0;
	//x = 2147483647;
	while (coords[++i].x != -1)
	{
		//if (coords[i].x)
		//{
			//fprintf(stderr, "x %d\n", coords->x);
			x = coords->x;
		//}
	}
	return (x);
}

t_place		*try_that(t_place *strt, t_place *finder, t_filler *map, char plr)
{
	int 		i;
	int 		j;
	t_place		tmp_check;
	t_place		try_fit;

	//tmp_check.y = 0;
	//tmp_check.x = 0;
	tmp_check = *strt;
	//try_fit = *finder;
	i = -1;
	j = -1;
	while (tmp_check.y < map->hcard)
	{
		while (tmp_check.x < map->wcard)
		{
			
			//try_fit.x = tmp_check.x - map->coords[0].x;
			try_fit.x = tmp_check.x - minus_x(map->coords); 
			try_fit.y = tmp_check.y - map->coords[0].y;
			//fprintf(stderr, "*try_that try_fit.y %d try_fit.x %d\n", try_fit.y, try_fit.x);
			if (try_fit_tetr(map, &try_fit, plr))
			{
				fprintf(stderr, "!!!!!!!!!(next_possible_pos)yes try_that try_fit.y %d try_fit.x %d\n", try_fit.y, try_fit.x);
				//*finder = try_fit;
				*strt = tmp_check;
				*finder = try_fit;				
				
				return (finder);
			}
			tmp_check.x++;
		}
		tmp_check.x = 0;
		tmp_check.y++;
	}
	return (NULL);
}


int	ft_power(int nb, int power)
{
	int i;
	int num;

	num = 1;
	i = -1;
	if (power == 0)
		return (1);
	else if (power < 0)
		return (0);
	while (++i < power)
		num *= nb;
	return (num);
}

int		ft_sqrt(int nbr)
{
	int	i;

	i = 0;
	while (i < 46341)
	{
		if (i * i == nbr)
			return (i);
		if (i * i > nbr)
			return (i);
		i++;
	}
	return (0);
}

//res possible coord    (tetri) my tetri coord   (tmp) enemy position
int		euclidian_dist(t_place *res, t_place *tetri, t_place *tmp)
{
	int 	y;
	int 	x;
	int 	sqrt;

	//fprintf(stderr, "** tmp.y %d tmp.x  %d res->y %d res->x %d\n", tmp->y, tmp->x, res->y, res->x);
	y = ft_power((res->y + tetri->y) - tmp->y, 2);
	x = ft_power((res->x + tetri->x) - tmp->x, 2);
	//fprintf(stderr, "y %d x %d\n", y, x);
	sqrt = ft_sqrt(y + x);
	return (sqrt);
}

int		distance(t_place *finder, t_filler *map, char plr2)
{
	int 		i;
	int 		ret;
	t_place		tmp;

	tmp.y= 0;
	tmp.x = 0;
	i = -1;
	ret = 0;
	while (map->coords[++i].x != -1)
	{
		while (tmp.y < map->hcard)
		{
			while (tmp.x < map->wcard)
			{
				if (map->card[tmp.y][tmp.x] == plr2)
				{
					fprintf(stderr, "(enemy) tmp.y %d tmp.x  %d (MY Attempt) finder->y %d finder->x %d\n", tmp.y, tmp.x, finder->y, finder->x);
					ret = euclidian_dist(finder, &map->coords[i], &tmp);
					return (ret);
				}
				tmp.x++;
			}
			tmp.x = 0;
			tmp.y++;
		}
	}
	return (-666);
}

int 	calc_diag_sides(t_place *finder, t_filler *map, char plr2)
{
	int 	i;
	int 	nbr;

	i = -1;
	nbr = 0;
	while (map->coords[++i].x != -1)
	{
		if (map->coords[i].y + finder->y + 1 < map->hcard && map->coords[i].x + finder->x + 1 < map->wcard && 
			map->card[map->coords[i].y + finder->y + 1][map->coords[i].x + finder->x + 1] == plr2)
			nbr++;
		if (map->coords[i].y + finder->y + 1 < map->hcard && map->coords[i].x + finder->x - 1 >= 0 && 
			map->card[map->coords[i].y + finder->y + 1][map->coords[i].x + finder->x - 1] == plr2)
			nbr++;
		if (map->coords[i].y + finder->y - 1 >= 0 && map->coords[i].x + finder->x + 1 < map->wcard && 
			map->card[map->coords[i].y + finder->y - 1][map->coords[i].x + finder->x + 1] == plr2)
			nbr++;
		if (map->coords[i].y + finder->y - 1 >= 0 && map->coords[i].x + finder->x - 1 >= 0 && 
			map->card[map->coords[i].y + finder->y - 1][map->coords[i].x + finder->x - 1] == plr2)
			nbr++;
	}
	return (nbr);
}

int 	calc_side(t_place *finder, t_filler *map, char plr2)
{
	int 	i;
	int 	nbr;

	i = -1;
	nbr = 0;
	while (map->coords[++i].x != -1)
	{
		if (map->coords[i].y + finder->y + 1 < map->hcard && 
			map->card[map->coords[i].y + finder->y + 1][map->coords[i].x + finder->x] == plr2)
			nbr++;
		if (map->coords[i].y + finder->y - 1 >= 0 && 
			map->card[map->coords[i].y + finder->y - 1][map->coords[i].x + finder->x] == plr2)
			nbr++;
		if (map->coords[i].x + finder->x + 1 < map->wcard && 
			map->card[map->coords[i].y + finder->y][map->coords[i].x + finder->x + 1] == plr2)
			nbr++;
		if (map->coords[i].x + finder->x - 1 >= 0 && 
			map->card[map->coords[i].y + finder->y][map->coords[i].x + finder->x - 1] == plr2)
			nbr++;
	}
	return (nbr);
}

int 	calc_sides_enemy(t_place *finder, t_filler *map, char plr2)
{
	int 	diag;
	int 	sides;

	diag = calc_diag_sides(finder, map, plr2);
	sides = calc_side(finder, map, plr2);
		fprintf(stderr, "calc_sides_enemy diag %d sides %d\n", diag, sides);
	return (diag < sides ? diag : sides);
}

int 	counter(t_place *finder, t_filler *map, char plr)
{
	int 	dist;
	int 	sides;

	dist = distance(finder, map, (plr == 'o' ? 'X' : 'O'));
		fprintf(stderr, "counter dist %d\n", dist);
	sides = calc_sides_enemy(finder, map, (plr == 'o' ? 'X' : 'O'));
		fprintf(stderr, "counter sides %d\n", sides);

	return (dist);
}

int		check_position(t_place *place, t_place *find, t_filler *map, char plr)
{
	//t_place		check;
	t_place		strt;
	t_place		finder;
	//t_place 	*min_res;
	int 		count;
	//int 		min_count;
	int 		val;

	//check = *place;
	//check.y = 0;
	//check.x = 0;
	//min_res = place;
	strt = *place;
	finder = *find;

	if (strt.x >= map->wcard)
	{
		strt.y += 1;
		strt.x = 0;
	}
	else
		strt.x += 1;
	//min_count = 0;
	if (try_that(&strt, &finder, map, plr) == NULL)
		return (-666);
	count = counter(&finder, map, plr);

	val = check_position(&strt, find, map, plr);
		fprintf(stderr, "val %d\n", val);
	if (count >= val)
	{
		fprintf(stderr, "@@@check_position ret finder.y %d finder.x %d | count %d val %d\n", finder.y, finder.x, count, val);
		*find = finder;		
		return (count);
	}
	//*find = str;//return FINAL value
	return (val);
}

int		main(void)
{
	char		plr;
	t_filler	map;
	t_place		place;
	t_place		find;

	find.y = 0;
	find.x = 0;
	if ((plr = get_player()) < 0)
		return (-1);
	fprintf(stderr, "plr %c\n", plr);
	/*if (parse_map(&map))
	{
		fprintf(stderr, "okkk\n");
	}*/
	//if (parse_tetr(&map))
	//	fprintf(stderr, "okkk\n");
	while (parse_map(&map) > -1 && parse_tetr(&map) > -1)
	{
	
		place.y = 0;
		place.x = -1;
		fprintf(stderr, "go\n");
		if (check_position(&place, &find, &map, plr) != -666)
		{
			ft_putnbr(find.y);
			ft_putchar(' ');
			ft_putnbr(find.x);
			ft_putchar('\n');
		}
		else
		{
			ft_putnbr(0);
			ft_putchar(' ');
			ft_putnbr(0);
			ft_putchar('\n');
		}
		free_map(&map);
	}
	
	
	//system("leaks amakhiny.filler");
	//fprintf(stderr, "plr %d\n", plr);

	return (0);
}