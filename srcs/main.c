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

	Elf64_Ehdr *ehdr = (Elf64_Ehdr *)map;
	Elf64_Shdr *shdr = (Elf64_Shdr *)((char *)map + ehdr->e_shoff);

	for (int i = 0; i < ehdr->e_shnum; i++)
	{
		if (shdr[i].sh_type == SHT_SYMTAB)
		{
			Elf64_Sym *symtab = (Elf64_Sym *)((char *)map + shdr[i].sh_offset);
			int symcount = shdr[i].sh_size / sizeof(Elf64_Sym);

			for (int j = 0; j < symcount; j++)
			{
				char buf[20];
				snprintf(buf, sizeof(buf), "%016lx", symtab[j].st_value);
				if (ft_strcmp(buf, "0000000000000000"))
				{
					ft_printf("%s\n", buf);
				}
			}
			break ;
		}
	}

	return (0);
}
