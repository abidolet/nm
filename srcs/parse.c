#include "ft_nm.h"
#include "libft/str.h"
#include "libft/ft_printf.h"

static void	print_options(const int fd, t_data *data)
{
	const char	*msg = "Usage: ./ft_nm [option(s)] [file(s)]\n"
	" List symbols in [file(s)] (a.out by default).\n"
	" The options are:\n"
	"  -a, --debug-syms\t Display debugger-only symbols\n"
	"  -A, --print-file-name\t Print name of the input file before every symbol\n"
	"  -e\t\t\t (ignored)\n"
	"  -g, --extern-only\t Display only external symbols\n"
	"  -n, --numeric-sort\t Sort symbols numerically by address\n"
	"  -o\t\t\t Same as -A\n"
	"  -p, --no-sort\t\t Do not sort the symbols\n"
	"  -r, --reverse-sort\t Reverse the sense of the sort\n"
	"  -u, --undefined-only\t Display only undefined symbols\n"
	"  -h, --help \t\t Display this information\n"
	"ft_nm: supported targets: elfx86_32 elfx86_64 binary object_files shared_object\n";

	ft_dprintf(fd, msg);
	if (fd == 1)
	{
		ft_printf("Report bugs to <abidolet@student.42lyon.fr>.\n");
	}

	data->exit_code = (fd == 2);
	free_all(data);
}

static e_options	get_option_enum(const char *opt_str, t_data *data)
{
	t_options	option_map[9] = {
		{"debug-syms", DEBUG_SYMS, FALSE},
		{"print-file-name", PRINT_FILE_NAME, FALSE},
		{"extern-only", EXTERN_ONLY, FALSE},
		{"numeric-sort", NUMERIC_SORT, FALSE},
		{"no-sort", NO_SORT, FALSE},
		{"reverse-sort", REVERSE_SORT, FALSE},
		{"undefined-only", UNDEF_ONLY, FALSE},
		{"help", HELP, FALSE},
	};

	const size_t	opt_size = ft_strlen(opt_str);
	e_options		option = INVALID;
	size_t			nb_options = 0;

	for (size_t i = 0; option_map[i].name; i++)
	{
		if (ft_strncmp(opt_str, option_map[i].name, opt_size) == 0)
		{
			option_map[i].selected = TRUE;
			option = option_map[i].opt;
			nb_options++;
		}
	}

	if (nb_options > 1)
	{
		ft_dprintf(2, "nm: option '--%s' is ambiguous; possibilities:", opt_str);
		for (size_t i = 0; option_map[i].name; i++)
		{
			if (option_map[i].selected)
			{
				ft_dprintf(2, " '--%s'", option_map[i].name);
			}
		}
		ft_dprintf(2, "\n");
		print_options(2, data);
	}

	return (option);
}

static void	parse_options(char *arg, t_data *data)
{
	if (*arg != '-')
	{
		while (*arg)
		{
			switch (*arg)
			{
				case 'a':	data->a = TRUE; break ;
				case 'A':	data->A = TRUE; break ;
				case 'e':	break ;
				case 'g':	data->g = TRUE; break ;
				case 'n':	data->n = TRUE; break ;
				case 'o':	data->A = TRUE; break ;
				case 'p':	data->p = TRUE; break ;
				case 'r':	data->r = TRUE; break ;
				case 'u':	data->u = TRUE; break ;
				case 'h':	print_options(1, data); break ;
				case 'H':	print_options(1, data); break ;
				default:	ft_dprintf(2, "ft_nm: invalid option -- '%c'\n", *arg);
								print_options(2, data);
			}
			arg++;
		}
	}
	else
	{
		switch (get_option_enum(arg + 1, data))
		{
			case DEBUG_SYMS:		data->a = TRUE; break ;
			case PRINT_FILE_NAME:	data->A = TRUE; break ;
			case EXTERN_ONLY:		data->g = TRUE; break ;
			case NUMERIC_SORT:		data->n = TRUE; break ;
			case NO_SORT:			data->p = TRUE; break ;
			case REVERSE_SORT:		data->r = TRUE; break ;
			case UNDEF_ONLY:		data->u = TRUE; break ;
			case HELP:				print_options(1, data); break ;
			default:				ft_dprintf(2, "ft_nm: unrecognized option '--%s'\n", arg + 1);
										print_options(2, data);
		}
	}
}

void	parse_data(const size_t argc, char **argv, t_data *data)
{
	t_list	*new_node = NULL;

	for (size_t i = 1; i < argc; i++)
	{
		if (*argv[i] == '-')
		{
			parse_options(argv[i] + 1, data);
		}
		else
		{
			check_mem(INFO, ft_strdup(argv[i]), (void **)&data->file, data);
			check_mem(INFO, ft_lstnew(data->file), (void **)&new_node, data);
			ft_lstadd_back(&data->files, new_node);
			data->file = NULL;
		}
	}

	data->nb_files = ft_lstsize(data->files);
	if (data->nb_files == 0)
	{
		check_mem(INFO, ft_strdup("a.out"), (void **)&data->file, data);
		check_mem(INFO, ft_lstnew(data->file), (void **)&new_node, data);
		ft_lstadd_back(&data->files, new_node);
		data->file = NULL;
	}
}
