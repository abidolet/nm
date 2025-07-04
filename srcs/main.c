#include <fcntl.h>
#include <sys/mman.h>
#include "libft/ft_printf.h"
#include "libft/str.h"
#include "libft/mem.h"
#include "libft/to.h"
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <unistd.h>
#include <libft/lst.h>
#include <ar.h>

#define FREE(ptr) free(ptr); ptr = NULL;

void	error(char *msg)
{
	ft_dprintf(2, "ft_nm: %s\n", msg);
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

int is_archive_file(int fd)
{
	char	magic[SARMAG];

	if (read(fd, magic, SARMAG) != SARMAG)
	{
		return (0);
	}
	return (ft_memcmp(magic, ARMAG, SARMAG) == 0);
}

void	print_line(void *content)
{
	char	**line = (char **)content;
	ft_printf("%s %s %s\n", line[0], line[1], line[2]);
}

void	free_line(void *content)
{
	char	**line = (char **)content;
	FREE(line[0]);
	FREE(line[1]);
	FREE(line);
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

	unsigned long	addr1 = ft_strtol(line1[0], NULL, 16);
	unsigned long	addr2 = ft_strtol(line2[0], NULL, 16);
	return (addr1 < addr2);
}

struct	flags
{
	t_list	*files;
	bool	a;
	bool	g;
	bool	p;
	bool	r;
	bool	u;
	int		exit_code;
	size_t	nb_files;
};

void	print_options(int fd)
{
	ft_dprintf(fd, "Usage: ./ft_nm [option(s)] [file(s)]\n");
	ft_dprintf(fd, " List symbols in [file(s)] (a.out by default).\n");
	ft_dprintf(fd, " The options are:\n");
	ft_dprintf(fd, "  -a, --debug-syms\t Display debugger-only symbols\n");
	ft_dprintf(fd, "  -e, \t\t\t (ignored)\n");
	ft_dprintf(fd, "  -g, --extern-only\t Display only external symbols\n");
	ft_dprintf(fd, "  -p, --no-sort\t\t Do not sort the symbols\n");
	ft_dprintf(fd, "  -r, --reverse-sort\t Reverse the sense of the sort\n");
	ft_dprintf(fd, "  -u, --undefined-only\t Display only undefined symbols\n");
	ft_dprintf(fd, "  -h, --help \t\t Display this information\n");
	ft_dprintf(fd, "ft_nm: supported targets: x86_32 x64 objectfiles .so\n");
	if (fd == 1)
	{
		ft_printf("Report bugs to <abidolet@student.42lyon.fr>.\n");
	}
}

void	parse_flags(int ac, char **av, struct flags *flags)
{
	for (int i = 1; i < ac; i++)
	{
		if (*av[i] == '-')
		{
			if (av[i][1] != '-')
			{
				char	*option = av[i] + 1;

				while (*option)
				{
					switch (*option)
					{
						case 'a':	flags->a = true; break ;
						case 'e':	break ;
						case 'g':	flags->g = true; break ;
						case 'p':	flags->p = true; break ;
						case 'r':	flags->r = true; break ;
						case 'u':	flags->u = true; break ;
						case 'h':	print_options(1);
										exit(0);
						default:	ft_dprintf(2, "ft_nm: invalid option -- '%c'\n", *option);
										print_options(2);
										exit(1);
					}
					option++;
				}
			}
			else
			{
				char	*option = av[i] + 2;

				if (ft_strcmp(option, "debug-syms") == 0)
				{
					flags->a = true;
				}
				else if (ft_strcmp(option, "extern-only") == 0)
				{
					flags->g = true;
				}
				else if (ft_strcmp(option, "no-sort") == 0)
				{
					flags->p = true;
				}
				else if (ft_strcmp(option, "reverse-sort") == 0)
				{
					flags->r = true;
				}
				else if (ft_strcmp(option, "undefined-only") == 0)
				{
					flags->u = true;
				}
				else if (ft_strcmp(option, "help") == 0)
				{
					print_options(1);
					exit(0);
				}
				else
				{
					ft_dprintf(2, "ft_nm: unrecognized option '--%s'\n", option);
					print_options(2);
					exit(1);
				}
			}
		}
		else
		{
			char	*file = ft_strdup(av[i]);
			t_list	*new_node = ft_lstnew(file);
			ft_lstadd_back(&flags->files, new_node);
		}
	}

	flags->nb_files = ft_lstsize(flags->files);
	if (flags->nb_files == 0)
	{
		char	*file = ft_strdup("a.out");
		t_list	*new_node = ft_lstnew(file);
		ft_lstadd_back(&flags->files, new_node);
	}
}

void	print_flags(struct flags *flags)
{
	ft_printf("Flags: a=%d, g=%d, u=%d, r=%d, p=%d nb_files=%d\n",
		flags->a, flags->g, flags->u, flags->r, flags->p, flags->nb_files);
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
	struct flags	flags;
	ft_bzero(&flags, sizeof(flags));
	parse_flags(ac, av, &flags);
	// print_flags(&flags);

	t_list	*tmp = flags.files;
	while (flags.files)
	{
		char	*file = flags.files->content;

		int	fd = open(file, O_RDONLY);
		if (fd < 0)
		{
			ft_dprintf(2, "ft_nm: '%s': No such file\n", file);
			flags.exit_code = 1;
			flags.files = flags.files->next;
			continue ;
		}

		struct stat	st;
		if (fstat(fd, &st))
		{
			perror("fstat failed");
			exit(1);
		}
		else if (S_ISDIR(st.st_mode))
		{
			ft_dprintf(2, "ft_nm: Warning: '%s' is a directory\n", file);
			close(fd);
			flags.exit_code = 1;
			flags.files = flags.files->next;
			continue ;
		}

		void	*map = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
		if (map == MAP_FAILED)
		{
			perror("mmap failed");
			exit(1);
		}

		Elf64_Ehdr	*ehdr = (Elf64_Ehdr *)map;
		if (ft_memcmp(ehdr->e_ident, ELFMAG, SELFMAG) != 0 && is_archive_file(fd))
		{
			ft_dprintf(2, "ft_nm: %s: file format not recognized\n", file);
			close(fd);
			exit(1);
		}

		if (flags.nb_files > 1)
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
					char		**line = ft_calloc(3, sizeof(char *));
					line[0] = ft_calloc(17, sizeof(char));
					line[1] = ft_calloc(2, sizeof(char));
					Elf64_Sym	*sym = &symtab[j];
					char		*strtab = (char *)map + shdr[shdr[i].sh_link].sh_offset;

					{
						snprintf(line[0], 17, "%016lx", symtab[j].st_value);
					}

					{
						*line[1] = get_symbol_type(&symtab[j], shdr, strtab);
						if (!ft_strcmp(line[0], "0000000000000000") && !(flags.a && *line[1] == 'a'))
						{
							if ((!flags.a && (*line[1] == 'A' || *line[1] == 't' || *line[1] == 'r')) || *line[1] == '?')
							{
								free_line(line);
								continue ;
							}
							if (*line[1] != 'T')
							{
								FREE(line[0]);
								line[0] = ft_strdup("                ");
							}
						}
					}

					{
						if (sym->st_name != 0)
						{
							line[2] = &strtab[sym->st_name];
						}
						else
						{
							line[2] = "";
						}
					}

					t_list	*new_node = ft_lstnew((void *)line);
					ft_lstadd_back(&lst, new_node);
				}
			}
		}

		if (!flags.a)
		{
			if (flags.u)
			{
				ft_lstremove_if(&lst, NULL, is_undefined_symbol, free_line);
			}
			else if (flags.g)
			{
				ft_lstremove_if(&lst, NULL, is_global_symbol, free_line);
			}
		}

		if (!flags.p)
		{
			lst = ft_lstsort(lst, sort_line);
			if (flags.r)
			{
				ft_lstreverse(&lst);
			}
		}

		ft_lstiter(lst, print_line);
		ft_lstclear(&lst, free_line);

		munmap(map, st.st_size);
		close(fd);

		flags.files = flags.files->next;
	}

	ft_lstclear(&tmp, free);
	return (flags.exit_code);
}
