#ifndef FT_NM_H
# define FT_NM_H

# include "libft/bool.h"
# include "libft/lst.h"
# include <unistd.h>
# include <stddef.h>

# define CLOSE(fd)	if (fd > 2) close(fd), fd = -1;
# define INFO		__FILE__, __LINE__, __func__

typedef struct data
{
	t_list	*files;
	char	*file;
	size_t	nb_files;
	char	**line;
	BOOL	a;
	BOOL	A;
	BOOL	g;
	BOOL	n;
	BOOL	p;
	BOOL	r;
	BOOL	u;
	int		exit_code;
	int		fd;
	t_list	*lines;
}	t_data;

typedef enum e_options
{
	DEBUG_SYMS,
	PRINT_FILE_NAME,
	EXTERN_ONLY,
	NUMERIC_SORT,
	NO_SORT,
	REVERSE_SORT,
	UNDEF_ONLY,
	HELP,
	INVALID
}	e_options;

typedef struct options
{
	const char	*name;
	e_options	opt;
	BOOL		selected;
}	t_options;

// utils.c
void	print_line(void *content);
void	free_line(void *content);
int		sort_line(void *content1, void *content2);
int		is_undefined_symbol(void *content);
int		is_global_symbol(void *content);
int		reverse_sort_line(void *content1, void *content2);
int		sort_line_adress(void *content1, void *content2);
int		reverse_sort_line_adress(void *content1, void *content2);
void	free_all(t_data *data);
void	check_mem(const char *file, const int line, const char *func,
			void *mem, void **ptr, t_data *data);

// parse.c
void	parse_data(const size_t argc, char **argv, t_data *data);

// run.c
void	run(t_data *data);

#endif
