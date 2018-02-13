/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakhiny <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/12 11:33:12 by amakhiny          #+#    #+#             */
/*   Updated: 2017/12/12 11:33:29 by amakhiny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "../includes/get_next_line.h"

/*static int	ft_lstremovecontent(t_list **list, int fd)
{
	t_list	*current;
	t_list	*previous;
	t_data	*data;

	previous = NULL;
	current = *list;
	while (current != NULL)
	{
		data = (t_data*)current->content;
		if (data->fd == fd)
		{
			if (previous == NULL)
				*list = current->next;
			else
				previous->next = current->next;
			free(data->content);
			free(data);
			free(current);
			return (0);
		}
		previous = current;
		current = current->next;
	}
	return (0);
}

static int	extend_content(t_data *data)
{
	int		i;
	char	*str;
	char	*result;

	i = 0;
	str = ft_strnew(BUFF_SIZE);
	if ((i = read(data->fd, str, BUFF_SIZE)) == -1)
		return (-1);
	if (data->content)
		result = ft_strjoin(data->content, str);
	else
		result = ft_strdup(str);
	free(data->content);
	free(str);
	data->content = result;
	if (i == 0 && !ft_strchr(data->content, '\n'))
		return (0);
	else
		return (1);
}

static int	parse(t_list **list, t_data *data, char **line)
{
	char	*str;
	int		i;

	while (1)
	{
		if (data->content && (str = ft_strchr(data->content, '\n')))
		{
			*str = '\0';
			*line = ft_strdup(data->content);
			ft_memmove(data->content, str + 1, ft_strlen(str + 1) + 1);
			return (1);
		}
		if ((i = extend_content(data)) == -1)
			return (-1);
		else if (i == 0)
		{
			if (data->end || data->content[0] == '\0')
				return (ft_lstremovecontent(list, data->fd));
			*line = ft_strdup(data->content);
			data->end = 1;
			return (1);
		}
	}
	return (-1);
}

int			get_next_line(const int fd, char **line)
{
	static t_list	*list;
	t_list			*cache;
	t_data			*data;

	cache = list;
	while (cache)
	{
		data = (t_data*)cache->content;
		if (data->fd == fd && line)
			return (parse(&list, data, line));
		cache = cache->next;
	}
	if (!(data = (t_data*)malloc(sizeof(t_data))) || !line)
		return (-1);
	data->fd = fd;
	data->content = NULL;
	data->end = 0;
	cache = ft_lstnew((void*)data, sizeof(t_data));
	free(data);
	if (list)
		ft_lstadd(&list, cache);
	else
		list = cache;
	return (get_next_line(fd, line));
}*/




static t_mem	*readbuff(t_mem *lst, const int fd)
{
	t_mem			*elem;

	elem = lst;
	while (elem)
	{
		if (elem->fd == fd)
			break ;
		elem = elem->next;
	}
	if (!elem)
	{
		if (!(elem = (t_mem *)malloc(sizeof(t_mem))))
			return (0);
		if (!(elem->content = ft_strnew(0)))
			return (0);
		elem->fd = fd;
		elem->next = NULL;
		while (lst && lst->next)
			lst = lst->next;
		if (lst)
			lst->next = elem;
	}
	return (elem);
}

static int		get_line(t_mem *lst, char **line)
{
	char	*temp;
	char	*ret;

	if (!(temp = ft_strdup(lst->content)))
		return (-1);
	free(lst->content);
	if (*temp == '\n')
	{
		if (!(*line = ft_strnew(0)) ||
			!(lst->content = ft_strdup(temp + 1)))
			return (-1);
		free(temp);
		return (1);
	}
	if ((ret = ft_strchr(temp, '\n')))
	{
		if (!(lst->content = ft_strdup(ret + 1)))
			return (-1);
		*ret = '\0';
	}
	else
		lst->content = ft_strnew(0);
	ret = temp;
	*line = ret;
	return (!*ret ? 0 : 1);
}

int				get_next_line(const int fd, char **line)
{
	static t_mem	*mem;
	t_mem			*lst;
	int				val;
	char			*box;
	char			buff[BUFF_SIZE + 1];

	if (fd < 0 || !line || !(lst = readbuff(mem, fd)) || BUFF_SIZE < 1)
		return (-1);
	if (!mem)
		mem = lst;
	while ((val = read(fd, buff, BUFF_SIZE)) > 0)
	{
		buff[val] = '\0';
		if (!(box = ft_strjoin(lst->content, buff)))
			return (-1);
		free(lst->content);
		lst->content = box;
		if (ft_strchr(lst->content, '\n'))
			return (get_line(lst, line));
	}
	if (val == -1)
		return (-1);
	return (get_line(lst, line));
}
