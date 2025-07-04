#include <fcntl.h>
#include <sys/mman.h>
#include "libft/ft_printf.h"
#include "libft/str.h"
#include "libft/mem.h"
#include "libft/to.h"
#include "libft/bool.h"
#include "libft/color.h"
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <unistd.h>
#include <libft/lst.h>
#include <ar.h>
#include <errno.h>

#define FREE(ptr) free(ptr); ptr = NULL;
#define CLOSE(fd) if (fd > 2) close(fd), fd = -1;
#define INFO __FILE__, __LINE__, __func__

typedef struct	data
{
	t_list	*files;
	char	*file;
	size_t	nb_files;
	char	**line;
	BOOL	a;
	BOOL	g;
	BOOL	p;
	BOOL	r;
	BOOL	u;
	int		exit_code;
	int		fd;
}	t_data;

void	free_all(t_data *data)
{
	FREE(data->file);
	if (data->files)
	{
		ft_lstclear(&data->files, free);
		data->files = NULL;
	}
	free_arr((void **)data->line);
	CLOSE(data->fd);
	exit(data->exit_code);
}

void	check_mem(const char *file, const int line, const char *func, void *mem, void **res, t_data *data)
{
	if (!mem)
	{
		ft_dprintf(2, "ft_nm: %s%s:%d: %smalloc assertion failed in %s'%s'\n",
			GRAY, file, line, RED, RESET, func);
		data->exit_code = 1;
		free_all(data);
	}
	*res = mem;
}

char	get_symbol_type(Elf64_Sym *sym, Elf64_Shdr *shdr, char *strtab)
{
	unsigned char	bind = ELF64_ST_BIND(sym->st_info);
	unsigned char	type = ELF64_ST_TYPE(sym->st_info);

	if (sym->st_name != 0 && !ft_strcmp(&strtab[sym->st_name], "data_start"))
	{
		return ('W');
	}

	if (sym->st_shndx == SHN_UNDEF && bind == STB_GLOBAL) return 'U';
	if (sym->st_shndx == SHN_ABS) return bind == STB_GLOBAL ? 'A' : 'a';
	if (sym->st_shndx == SHN_COMMON) return 'C';

	if (bind == STB_WEAK)
	{
		if (type == STT_OBJECT)
		{
			return (bind == STB_GLOBAL ? 'V' : 'v');
		}
		else
		{
			return (bind == STB_GLOBAL ? 'W' : 'w');
		}
	}

	if (sym->st_shndx < SHN_LORESERVE)
	{
		Elf64_Shdr	*sec = &shdr[sym->st_shndx];

		if (sec->sh_flags & SHF_EXECINSTR) return bind == STB_GLOBAL ? 'T' : 't';
		if (sec->sh_type == SHT_NOBITS) return bind == STB_GLOBAL ? 'B' : 'b';
		if (sec->sh_flags & SHF_WRITE) return bind == STB_GLOBAL ? 'D' : 'd';
		if (sec->sh_flags & SHF_ALLOC) return bind == STB_GLOBAL ? 'R' : 'r';
	}

	return ('?');
}

void	print_line(void *content)
{
	char	**line = (char **)content;
	ft_printf("%s %s %s\n", line[0], line[1], line[2]);
}

void	free_line(void *content)
{
	char	**line = (char **)content;
	free_arr((void **)line);
}

int	sort_line(void *content1, void *content2)
{
	char	**line1 = (char **)content1;
	char	**line2 = (char **)content2;

	int	name_cmp = ft_strcmp(line1[2], line2[2]);
	if (name_cmp != 0)
	{
		return (name_cmp);
	}

	unsigned long	addr1 = strtoul(line1[0], NULL, 16);
	unsigned long	addr2 = strtoul(line2[0], NULL, 16);
	return (addr1 < addr2);
}

void	print_options(const int fd, t_data *data)
{
	char	*msg = "Usage: ./ft_nm [option(s)] [file(s)]\n"
		" List symbols in [file(s)] (a.out by default).\n"
		" The options are:\n"
		"  -a, --debug-syms\t Display debugger-only symbols\n"
		"  -e, \t\t\t (ignored)\n"
		"  -g, --extern-only\t Display only external symbols\n"
		"  -p, --no-sort\t\t Do not sort the symbols\n"
		"  -r, --reverse-sort\t Reverse the sense of the sort\n"
		"  -u, --undefined-only\t Display only undefined symbols\n"
		"  -h, --help \t\t Display this information\n"
		"ft_nm: supported targets: x86_32 x64 objectfiles .so\n";

	ft_dprintf(fd, msg);
	if (fd == 1)
	{
		ft_printf("Report bugs to <abidolet@student.42lyon.fr>.\n");
	}
	data->exit_code = (fd == 2);
	free_all(data);
}

void	parse_flags(const int ac, char **av, t_data *data)
{
	t_list	*new_node = NULL;
	char	*option;

	for (int i = 1; i < ac; i++)
	{
		if (*av[i] == '-')
		{
			if (av[i][1] != '-')
			{
				option = av[i] + 1;

				while (*option)
				{
					switch (*option)
					{
						case 'a':	data->a = TRUE; break ;
						case 'e':	break ;
						case 'g':	data->g = TRUE; break ;
						case 'p':	data->p = TRUE; break ;
						case 'r':	data->r = TRUE; break ;
						case 'u':	data->u = TRUE; break ;
						case 'h':	print_options(1, data); break ;
						default:	ft_dprintf(2, "ft_nm: invalid option -- '%c'\n", *option); print_options(2, data);
					}
					option++;
				}
			}
			else
			{
				option = av[i] + 2;

				if (ft_strcmp(option, "debug-syms") == 0)
				{
					data->a = TRUE;
				}
				else if (ft_strcmp(option, "extern-only") == 0)
				{
					data->g = TRUE;
				}
				else if (ft_strcmp(option, "no-sort") == 0)
				{
					data->p = TRUE;
				}
				else if (ft_strcmp(option, "reverse-sort") == 0)
				{
					data->r = TRUE;
				}
				else if (ft_strcmp(option, "undefined-only") == 0)
				{
					data->u = TRUE;
				}
				else if (ft_strcmp(option, "help") == 0)
				{
					print_options(1, data);
				}
				else
				{
					ft_dprintf(2, "ft_nm: unrecognized option '--%s'\n", option);
					print_options(2, data);
				}
			}
		}
		else
		{
			check_mem(INFO, ft_strdup(av[i]), (void **)&data->file, data);
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

void	print_flags(struct data *data)
{
	ft_printf("Flags: a=%d, g=%d, u=%d, r=%d, p=%d nb_files=%d\n",
		data->a, data->g, data->u, data->r, data->p, data->nb_files);
}

int	is_undefined_symbol(void *content, void *ref)
{
	char	**line = (char **)content;

	(void)ref;
	return (*line[1] == 'U' || *line[1] == 'w');
}

int	is_global_symbol(void *content, void *ref)
{
	char	**line = (char **)content;

	(void)ref;
	return (!(*line[1] == 't' || *line[1] == 'd' || *line[1] == 'b' || *line[1] == 'r'));
}

int	main(int ac, char **av)
{
	struct data	data;
	ft_bzero(&data, sizeof(data));
	parse_flags(ac, av, &data);
	// print_flags(&data);

	t_list	*tmp = data.files;
	while (data.files)
	{
		char	*file = data.files->content;

		data.fd = open(file, O_RDONLY);
		if (data.fd < 0)
		{
			data.exit_code = 1;
			if (errno == ENOENT)
			{
				ft_dprintf(2, "ft_nm: '%s': No such file\n", file);
			}
			else if (errno == EACCES)
			{
				ft_dprintf(2, "ft_nm: %s: Permission denied\n", file);
			}
			else
			{
				perror("open failed");
				free_all(&data);
			}
			data.files = data.files->next;
			continue ;
		}

		struct stat	st;
		if (fstat(data.fd, &st))
		{
			perror("fstat failed");
			data.exit_code = 1;
			free_all(&data);
		}
		else if (S_ISDIR(st.st_mode))
		{
			ft_dprintf(2, "ft_nm: Warning: '%s' is a directory\n", file);
			CLOSE(data.fd);
			data.exit_code = 1;
			data.files = data.files->next;
			continue ;
		}

		void	*map = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, data.fd, 0);
		CLOSE(data.fd);
		if (map == MAP_FAILED)
		{
			data.exit_code = 1;
			data.files = data.files->next;
			continue ;
		}

		Elf64_Ehdr	*ehdr = (Elf64_Ehdr *)map;
		if (ft_memcmp(ehdr->e_ident, ELFMAG, SELFMAG) != 0)
		{
			ft_dprintf(2, "ft_nm: %s: file format not recognized\n", file);
			data.exit_code = 1;
			data.files = data.files->next;
			continue ;
		}

		if (data.nb_files > 1)
		{
			ft_printf("\n%s:\n", file);
		}

		Elf64_Shdr	*shdr = (Elf64_Shdr *)((char *)map + ehdr->e_shoff);
		t_list		*lst = NULL;

		for (int i = 0; i < ehdr->e_shnum; i++)
		{
			if (shdr[i].sh_type == SHT_SYMTAB)
			{
				Elf64_Sym	*symtab = (Elf64_Sym *)((char *)map + shdr[i].sh_offset);
				int			symcount = shdr[i].sh_size / sizeof(Elf64_Sym);

				for (int j = 0; j < symcount; j++)
				{
					check_mem(INFO, ft_calloc(4, sizeof(char *)), (void **)&data.line, &data);
					check_mem(INFO, ft_calloc(17, sizeof(char)), (void **)&data.line[0], &data);
					check_mem(INFO, ft_calloc(2, sizeof(char)), (void **)&data.line[1], &data);

					Elf64_Sym	*sym = &symtab[j];
					char		*strtab = (char *)map + shdr[shdr[i].sh_link].sh_offset;

					{
						snprintf(data.line[0], 17, "%016lx", symtab[j].st_value);
					}

					{
						*data.line[1] = get_symbol_type(&symtab[j], shdr, strtab);
						if (!ft_strcmp(data.line[0], "0000000000000000") && !(data.a && *data.line[1] == 'a'))
						{
							if (*data.line[1] == 'a' || *data.line[1] == 'A' || *data.line[1] == 't' || *data.line[1] == 'r' || *data.line[1] == '?')
							{
								free_line(data.line);
								continue ;
							}
							if (*data.line[1] != 'T')
							{
								ft_memset(data.line[0], ' ', 16);
							}
						}
					}

					{
						check_mem(INFO, ft_strdup(&strtab[sym->st_name]), (void **)&data.line[2], &data);
					}

					t_list	*new_node = NULL;
					check_mem(INFO, ft_lstnew(data.line), (void **)&new_node, &data);
					ft_lstadd_back(&lst, new_node);
				}
			}
		}

		if (!data.a)
		{
			if (data.u)
			{
				ft_lstremove_if(&lst, NULL, is_undefined_symbol, free_line);
			}
			else if (data.g)
			{
				ft_lstremove_if(&lst, NULL, is_global_symbol, free_line);
			}
		}

		if (!data.p)
		{
			lst = ft_lstsort(lst, sort_line);
			if (data.r)
			{
				ft_lstreverse(&lst);
			}
		}

		ft_lstiter(lst, print_line);
		ft_lstclear(&lst, free_line);

		munmap(map, st.st_size);
		data.files = data.files->next;
	}

	ft_lstclear(&tmp, free);
	return (data.exit_code);
}
