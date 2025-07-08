#include "ft_nm.h"
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdio.h>
#include <elf.h>
#include <errno.h>
#include <stdlib.h>
#include "libft/mem.h"
#include "libft/str.h"
#include "libft/ft_printf.h"

static void	handle_options(t_data *data, t_list **lst)
{
	if (data->u)
	{
		ft_lstremove_if(lst, is_undefined_symbol, free_line);
	}
	else if (data->g)
	{
		ft_lstremove_if(lst, is_global_symbol, free_line);
	}

	if (data->n)
	{
		if (data->r)
		{
			*lst = ft_lstsort(*lst, reverse_sort_line_adress);
		}
		else
		{
			*lst = ft_lstsort(*lst, sort_line_adress);
		}
	}
	else if (!data->p)
	{
		if (data->r)
		{
			*lst = ft_lstsort(*lst, reverse_sort_line);
		}
		else
		{
			*lst = ft_lstsort(*lst, sort_line);
		}
	}
}

static char	get_symbol_type(void *sym, void *shdr, BOOL is_elf32)
{
	unsigned char	bind, type;
	unsigned short	shndx;
	unsigned long	sh_flags, sh_type;

	if (is_elf32)
	{
		Elf32_Sym	*sym32 = (Elf32_Sym *)sym;
		bind = ELF32_ST_BIND(sym32->st_info);
		type = ELF32_ST_TYPE(sym32->st_info);
		shndx = sym32->st_shndx;
	}
	else
	{
		Elf64_Sym	*sym64 = (Elf64_Sym *)sym;
		bind = ELF64_ST_BIND(sym64->st_info);
		type = ELF64_ST_TYPE(sym64->st_info);
		shndx = sym64->st_shndx;
	}

	if (bind == STB_WEAK)
	{
		if (type == STT_OBJECT)
			return (shndx == SHN_UNDEF ? 'v' : 'V');
		else
			return (shndx == SHN_UNDEF ? 'w' : 'W');
	}
	else if (shndx == SHN_UNDEF && bind == STB_GLOBAL)
		return ('U');
	else if (bind == STB_GNU_UNIQUE)
		return ('u');
	else if (shndx == SHN_ABS)
		return (bind == STB_GLOBAL ? 'A' : 'a');
	else if (shndx == SHN_COMMON)
		return ('C');
	else if (shndx < SHN_LORESERVE)
	{
		if (is_elf32)
		{
			sh_flags = ((Elf32_Shdr *)shdr)[shndx].sh_flags;
			sh_type = ((Elf32_Shdr *)shdr)[shndx].sh_type;
		}
		else
		{
			sh_flags = ((Elf64_Shdr *)shdr)[shndx].sh_flags;
			sh_type = ((Elf64_Shdr *)shdr)[shndx].sh_type;
		}
		if (sh_flags & SHF_EXECINSTR)
			return (bind == STB_GLOBAL ? 'T' : 't');
		else if (sh_type == SHT_NOBITS)
			return (bind == STB_GLOBAL ? 'B' : 'b');
		if (sh_flags & SHF_WRITE)
			return (bind == STB_GLOBAL ? 'D' : 'd');
		if (sh_flags & SHF_ALLOC)
			return (bind == STB_GLOBAL ? 'R' : 'r');
	}

	return (is_elf32 ? 'N' : 'n');
}

void	get_adress(char *buffer, size_t size, size_t value)
{
	char	hex_digit;

	ft_memset(buffer, '0', size);
	while (value > 0)
	{
		size--;
		hex_digit = value % 16;
		if (hex_digit < 10)
			buffer[size] = '0' + hex_digit;
		else
			buffer[size] = 'a' + (hex_digit - 10);
		value /= 16;
	}
}

static void	get_symbols_elf32(void *map, Elf32_Ehdr *ehdr, t_data *data, t_list **lst)
{
	Elf32_Shdr	*shdr = (Elf32_Shdr *)((char *)map + ehdr->e_shoff);

	for (int i = 0; i < ehdr->e_shnum; i++)
	{
		if (shdr[i].sh_type == SHT_SYMTAB)
		{
			Elf32_Sym	*symtab = (Elf32_Sym *)((char *)map + shdr[i].sh_offset);
			int			symcount = shdr[i].sh_size / sizeof(Elf32_Sym);

			for (int j = 0; j < symcount; j++)
			{
				data->line = NULL;
				check_mem(INFO, ft_calloc(5, sizeof(char *)), (void **)&data->line, data);
				check_mem(INFO, ft_calloc(9, sizeof(char)), (void **)&data->line[0], data);
				check_mem(INFO, ft_calloc(2, sizeof(char)), (void **)&data->line[1], data);

				Elf32_Sym	*sym = &symtab[j];
				char		*strtab = (char *)map + shdr[shdr[i].sh_link].sh_offset;

				{
					get_adress(data->line[0], 8, symtab[j].st_value);
				}

				{
					*data->line[1] = get_symbol_type(&symtab[j], shdr, TRUE);
					if (!ft_strcmp(data->line[0], "00000000")
						&& !(data->a && *data->line[1] == 'a'))
					{
						if (*data->line[1] == 'a')
						{
							free_line(data->line);
							continue ;
						}
						else if (is_undefined_symbol(data->line))
						{
							ft_memset(data->line[0], ' ', 8);
						}
					}
				}

				{
					if (sym->st_name == 0)
					{
						if (data->a)
						{
							char	*name = "";
							if (sym->st_shndx < ehdr->e_shnum)
							{
								name = (char *)map + shdr[ehdr->e_shstrndx].sh_offset + shdr[sym->st_shndx].sh_name;
							}
							check_mem(INFO, ft_strdup(name), (void **)&data->line[2], data);
							if (!*name && *data->line[1] == 'N')
							{
								free_line(data->line);
								continue ;
							}
						}
						else
						{
							free_line(data->line);
							continue ;
						}
					}
					else
					{
						check_mem(INFO, ft_strdup(&strtab[sym->st_name]), (void **)&data->line[2], data);
					}
				}

				{
					if (data->A)
					{
						check_mem(INFO, ft_strdup(data->file), (void **)&data->line[3], data);
					}
				}

				t_list	*new_node = NULL;
				check_mem(INFO, ft_lstnew(data->line), (void **)&new_node, data);
				ft_lstadd_back(lst, new_node);
			}
		}
	}
}

static void	get_symbols_elf64(void *map, Elf64_Ehdr *ehdr, t_data *data, t_list **lst)
{
	Elf64_Shdr	*shdr = (Elf64_Shdr *)((char *)map + ehdr->e_shoff);

	for (int i = 0; i < ehdr->e_shnum; i++)
	{
		if (shdr[i].sh_type == SHT_SYMTAB)
		{
			Elf64_Sym	*symtab = (Elf64_Sym *)((char *)map + shdr[i].sh_offset);
			int			symcount = shdr[i].sh_size / sizeof(Elf64_Sym);

			for (int j = 0; j < symcount; j++)
			{
				data->line = NULL;
				check_mem(INFO, ft_calloc(5, sizeof(char *)), (void **)&data->line, data);
				check_mem(INFO, ft_calloc(17, sizeof(char)), (void **)&data->line[0], data);
				check_mem(INFO, ft_calloc(2, sizeof(char)), (void **)&data->line[1], data);

				Elf64_Sym	*sym = &symtab[j];
				char		*strtab = (char *)map + shdr[shdr[i].sh_link].sh_offset;

				{
					get_adress(data->line[0], 16, symtab[j].st_value);
				}

				{
					*data->line[1] = get_symbol_type(&symtab[j], shdr, FALSE);
					if (!ft_strcmp(data->line[0], "0000000000000000")
						&& !(data->a && *data->line[1] == 'a'))
					{
						if (*data->line[1] == 'a')
						{
							free_line(data->line);
							continue ;
						}
						else if (is_undefined_symbol(data->line))
						{
							ft_memset(data->line[0], ' ', 16);
						}
					}
				}

				{
					if (sym->st_name == 0)
					{
						if (data->a)
						{
							char	*name = "";
							if (sym->st_shndx < ehdr->e_shnum)
							{
								name = (char *)map + shdr[ehdr->e_shstrndx].sh_offset + shdr[sym->st_shndx].sh_name;
							}
							check_mem(INFO, ft_strdup(name), (void **)&data->line[2], data);
							if (!*name && *data->line[1] == 'n')
							{
								free_line(data->line);
								continue ;
							}
						}
						else
						{
							free_line(data->line);
							continue ;
						}
					}
					else
					{
						check_mem(INFO, ft_strdup(&strtab[sym->st_name]), (void **)&data->line[2], data);
					}
				}

				{
					if (data->A)
					{
						check_mem(INFO, ft_strdup(data->file), (void **)&data->line[3], data);
					}
				}

				t_list	*new_node = NULL;
				check_mem(INFO, ft_lstnew(data->line), (void **)&new_node, data);
				ft_lstadd_back(lst, new_node);
			}
		}
	}
}

static void	handle_open_failed(const char *file, t_data *data)
{
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
		free_all(data);
	}
}

void	run(t_data *data)
{
	t_list	*tmp = data->files;

	while (tmp)
	{
		data->file = tmp->content;
		data->fd = open(data->file, O_RDONLY);
		if (data->fd < 0)
		{
			data->exit_code++;
			tmp = tmp->next;
			handle_open_failed(data->file, data);
			continue ;
		}

		struct stat	st;
		if (fstat(data->fd, &st))
		{
			perror("fstat failed");
			if (!data->exit_code) data->exit_code = 1;
			free_all(data);
		}
		else if (S_ISDIR(st.st_mode))
		{
			ft_dprintf(2, "ft_nm: Warning: '%s' is a directory\n", data->file);
			CLOSE(data->fd);
			data->exit_code++;
			tmp = tmp->next;
			continue ;
		}

		void	*map = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, data->fd, 0);
		CLOSE(data->fd);
		if (map == MAP_FAILED)
		{
			data->exit_code++;
			tmp = tmp->next;
			continue ;
		}

		data->lines = NULL;
		Elf32_Ehdr	*ehdr32 = (Elf32_Ehdr *)map;
		Elf64_Ehdr	*ehdr64 = (Elf64_Ehdr *)map;
		if (ehdr32->e_ident[EI_CLASS] == ELFCLASS32)
		{
			get_symbols_elf32(map, ehdr32, data, &data->lines);
		}
		else if (ehdr64->e_ident[EI_CLASS] == ELFCLASS64)
		{
			get_symbols_elf64(map, ehdr64, data, &data->lines);
		}
		else
		{
			ft_dprintf(2, "ft_nm: %s: data->file format not recognized\n", data->file);
			data->exit_code++;
			munmap(map, st.st_size);
			tmp = tmp->next;
			continue ;
		}

		handle_options(data, &data->lines);

		if (data->nb_files > 1 && !data->A)
		{
			ft_printf("\n%s:\n", data->file);
		}

		if (data->lines)
		{
			ft_lstiter(data->lines, print_line);
			ft_lstclear(&data->lines, free_line);
		}
		else if (!data->u && !data->g)
		{
			ft_dprintf(2, "ft_nm: %s: no symbols\n", data->file);
		}

		munmap(map, st.st_size);
		tmp = tmp->next;
	}
	ft_lstclear(&data->files, free);
}
