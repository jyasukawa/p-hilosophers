#include "philo.h"

ssize_t	ft_str_to_nbr(char *str)
{
	size_t		i;
	long long	nbr;

	i = 0;
	nbr = 0;
	while (str[i] != '\0')
	{
		if (str[i] >= '0' && str[i] <= '9')
		{
			if ((INT_MAX - (str[i] - '0')) / 10 < nbr)
				return (-1);
			nbr = (nbr * 10) + (str[i] - '0');
			i++;
		}
		else
			return (-1);
	}
	return (nbr);
}

static int	ft_is_valid_as_a_number(char **argv)
{
	size_t	i;

	i = 1;
	while (argv[i] != NULL)
	{
		if (ft_str_to_nbr(argv[i]) == -1 || ft_str_to_nbr(argv[i]) == 0)
		{
			printf("Error, Invalid arguments\n");
			return (-1);
		}
		i++;
	}
	return (0);
}

static void	ft_put_usage(void)
{
	printf("Usage: ./philo [1] [2] [3] [4] (5)\n");
	printf("  [1]: number_of_philosophers\n");
	printf("  [2]: time_to_die\n");
	printf("  [3]: time_to_eat\n");
	printf("  [4]: time_to_sleep\n");
	printf("  (5): number_of_times_each_philosopher_must_eat (optional)\n");
}

int	ft_check_args(int argc, char **argv)
{
	if (argc != 5 && argc != 6)
	{
		ft_put_usage();
		return (-1);
	}
	if (ft_is_valid_as_a_number(argv) == -1)
		return (-1);
	return (0);
}
