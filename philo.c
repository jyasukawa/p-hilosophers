#include "philo.h"

static void	ft_init_args(int argc, char **argv, t_all *all)
{
	all->nbr_of_philo = ft_str_to_nbr(argv[1]);
	all->time_to_die = ft_str_to_nbr(argv[2]);
	all->time_to_eat = ft_str_to_nbr(argv[3]);
	all->time_to_sleep = ft_str_to_nbr(argv[4]);
	if (argc == 6)
		all->must_eat_count = ft_str_to_nbr(argv[5]);
}

static void	ft_end_threads(t_all *all)
{
	size_t	i;

	if (all->nbr_of_philo == 1)
		pthread_mutex_unlock(&all->forks_head[0]);//philoが一人の場合のみ、eatingが実行されないため、forkはunlockされていないまま。
	i = 0;
	while (i < all->nbr_of_philo)
	{
		pthread_join(all->threads_head[i], NULL);
		i++;
	}
}

int	main(int argc, char **argv)
{
	t_all	all;

	if (ft_check_args(argc, argv) == -1)
		return (1);
	all = (t_all){0};
	ft_init_args(argc, argv, &all);
	if (ft_init_mutexes(&all) == -1)
		return (1);
	if (ft_init_philos(&all) == -1)
		return (1);
	if (ft_exec_multi_thread(&all) == -1)
		return (1);
	ft_end_threads(&all);
	ft_clean_up(&all);
	return (0);
}
