#include "ft_nm.h"
#include "libft/color.h"
#include "libft/ft_printf.h"
#include "libft/str.h"
#include <stdlib.h>

void	print_line(void *content)
{
	const char	**line = (const char **)content;

	if (line[3])
	{
		ft_printf("%s:", line[3]);
	}

	ft_printf("%s %s %s\n", line[0], line[1], line[2]);
}

void	free_line(void *content)
{
	free_arr((void **)content);
}

int	reverse_sort_line_adress(void *content1, void *content2)
{
	return (-sort_line_adress(content1, content2));
}

int	sort_line_adress(void *content1, void *content2)
{
	const char	**line1 = (const char **)content1;
	const char	**line2 = (const char **)content2;

	const int	addr_comp = ft_strcmp(line1[0], line2[0]);
	if (addr_comp != 0)
	{
		return (addr_comp);
	}

	return (ft_strcmp(line1[2], line2[2]));
}

int	sort_line(void *content1, void *content2)
{
	const char	**line1 = (const char **)content1;
	const char	**line2 = (const char **)content2;

	const int	name_cmp = ft_strcmp(line1[2], line2[2]);
	if (name_cmp != 0)
	{
		return (name_cmp);
	}

	const int	type_cmp = ft_strcmp(line1[1], line2[1]);
	if (type_cmp != 0)
	{
		return (-type_cmp);
	}

	return (1);
}

int	is_undefined_symbol(void *content)
{
	const char	**line = (const char **)content;
	const char	*defined_symbols = "CUvw";

	return (ft_strchr(defined_symbols, *line[1]) != NULL);
}

int	is_global_symbol(void *content)
{
	const char	**line = (const char **)content;
	const char	*globals_symbols = "ABDGRTUVWuvw";

	return (ft_strchr(globals_symbols, *line[1]) != NULL);
}

int	reverse_sort_line(void *content1, void *content2)
{
	const char	**line1 = (const char **)content1;
	const char	**line2 = (const char **)content2;

	if (ft_strcmp(line1[2], line2[2]) == 0)
	{
		return (sort_line(content1, content2));
	}

	return (-sort_line(content1, content2));
}

void	free_all(t_data *data)
{
	ft_lstclear(&data->files, free);
	free_arr((void **)data->line);
	ft_lstclear(&data->lines, free_line);
	CLOSE(data->fd);
	exit(data->exit_code);
}

void	check_mem(const char *file, const int line, const char *func,
	void *mem, void **ptr, t_data *data)
{
	if (!mem)
	{
		ft_dprintf(2, "ft_nm: %s%s:%d: %smalloc assertion failed in %s'%s'\n",
			GRAY, file, line, RED, RESET, func);
		data->exit_code = 1;
		free_all(data);
	}
	*ptr = mem;
}
