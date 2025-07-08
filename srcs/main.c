#include "ft_nm.h"
#include "libft/mem.h"

int	main(int argc, char **argv)
{
	t_data	data;

	ft_bzero(&data, sizeof(data));
	parse_data(argc, argv, &data);
	run(&data);
	return (data.exit_code);
}
