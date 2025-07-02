#include <fcntl.h>
#include <sys/mman.h>
#include "libft/ft_printf.h"
#include "libft/str.h"
#include "libft/mem.h"
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <unistd.h>

void	error(char *msg)
{
	perror(msg);
	exit(1);
}

char	 get_symbol_type(Elf64_Sym *sym, Elf64_Shdr *shdr, char *strtab)
{
	unsigned char	bind = ELF64_ST_BIND(sym->st_info);
	unsigned char	type = ELF64_ST_TYPE(sym->st_info);

	if (sym->st_name != 0 && !ft_strcmp(&strtab[sym->st_name], "data_start"))
	{
		return ('W');
	}

	if (sym->st_shndx == SHN_UNDEF && bind == STB_GLOBAL) return 'U';
	if (sym->st_shndx == SHN_ABS) return 'A';
	if (sym->st_shndx == SHN_COMMON) return 'C';

	if (bind == STB_WEAK)
	{
		return (type == STT_OBJECT) ? 'v' : bind == STB_GLOBAL ? 'W' : 'w';
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

int	main(int ac, char **av)
{
	char	*file = (ac == 1 ? "a.out" : av[1]);

	int	fd = open(file, O_RDONLY);
	if (fd < 0)
	{
		ft_dprintf(2, "ft_nm: '%s': No such file\n", file);
		return (1);
	}

	struct stat	st;
	if (fstat(fd, &st))
	{
		error("fstat failed");
	}

	void	*map = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (map == MAP_FAILED)
	{
		error("mmap failed");
	}

	Elf64_Ehdr	*ehdr = (Elf64_Ehdr *)map;
	Elf64_Shdr	*shdr = (Elf64_Shdr *)((char *)map + ehdr->e_shoff);

	for (int i = 0; i < ehdr->e_shnum; i++)
	{
		if (shdr[i].sh_type == SHT_SYMTAB)
		{
			Elf64_Sym	*symtab = (Elf64_Sym *)((char *)map + shdr[i].sh_offset);
			int			symcount = shdr[i].sh_size / sizeof(Elf64_Sym);

			for (int j = 0; j < symcount; j++)
			{
				char		buf[20];
				Elf64_Sym	*sym = &symtab[j];
				char		*strtab = (char *)map + shdr[shdr[i].sh_link].sh_offset;

				{
					snprintf(buf, sizeof(buf), "%016lx", symtab[j].st_value);
					if (ft_strcmp(buf, "0000000000000000"))
					{
						ft_printf("%s", buf);
					}
				}

				{
					char	c = get_symbol_type(&symtab[j], shdr, strtab);
					if (!ft_strcmp(buf, "0000000000000000"))
					{
						if (c == 'A' || c == '?')
						{
							continue ;
						}
						ft_printf("                ");
					}
					ft_printf(" %c ", c);
				}

				if (sym->st_name != 0)
				{
					char	*name = &strtab[sym->st_name];
					ft_printf("%s", name);
				}

				ft_printf("\n");
			}
			break ;
		}
	}

	return (0);
}
