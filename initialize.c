#include "philo.h"

static int	ft_destroy_mutexes(t_all *all)
{
	pthread_mutex_destroy(&all->put_status);
	pthread_mutex_destroy(&all->count_full);
	pthread_mutex_destroy(&all->manage_flag);
	return (-1);
}

static int	ft_init_eaten(t_all *all)
{
	size_t					i;
	pthread_mutex_t	*meals;

	meals = malloc(sizeof(pthread_mutex_t) * all->nbr_of_philo);
	if (meals == NULL)
		return (ft_destroy_mutexes(all));
	i = 0;
	while (i < all->nbr_of_philo)
	{
		if (pthread_mutex_init(&meals[i], NULL) != 0)
		{
			ft_destroy_mutexes(all);
			ft_destroy_mutex_until_index(i, meals);
			free(meals);//freeするのはdestroyした後。
			return (-1);
		}
		i++;
	}
	all->meals_head = meals;
	return (0);
}

static int	ft_init_forks(t_all *all)
{
	size_t					i;
	pthread_mutex_t	*forks;

	forks = malloc(sizeof(pthread_mutex_t) * all->nbr_of_philo);
	if (forks == NULL)
		return (ft_destroy_mutexes(all));
	i = 0;
	while (i < all->nbr_of_philo)
	{
		if (pthread_mutex_init(&forks[i], NULL) != 0)
		{
			ft_destroy_mutexes(all);
			ft_destroy_mutex_until_index(i, forks);
			free(forks);//freeするのはdestroyした後。
			return (-1);
		}
		i++;
	}
	all->forks_head = forks;
	return (0);
}

int	ft_init_mutexes(t_all *all)
{
	if (pthread_mutex_init(&all->put_status, NULL) != 0)
		return (-1);
	if (pthread_mutex_init(&all->count_full, NULL) != 0)
	{
		pthread_mutex_destroy(&all->put_status);
		return (-1);
	}
	if (pthread_mutex_init(&all->manage_flag, NULL) != 0)
	{
		pthread_mutex_destroy(&all->put_status);
		pthread_mutex_destroy(&all->count_full);
		return (-1);
	}
	if (ft_init_forks(all) == -1)
		return (-1);
	if (ft_init_eaten(all) == -1)
	{
		ft_destroy_mutex_until_index(all->nbr_of_philo, all->forks_head);
		free(all->forks_head);
		return (-1);
	}
	return (0);
}

int	ft_init_philos(t_all *all)
{
	size_t	i;
	t_philo	*philos;

	philos = (t_philo *)malloc(sizeof(t_philo) * all->nbr_of_philo);
	if (philos == NULL)
		return (ft_clean_up(all));
	i = 0;
	while (i < all->nbr_of_philo)
	{
		philos[i].philo_id = i;//ややこしくなるので、出力時以外は最初のphiloを1ではなく0と名付ける。
		philos[i].l_fork = &all->forks_head[i];
		if (i == all->nbr_of_philo - 1)
			philos[i].r_fork = &all->forks_head[0];
		else
			philos[i].r_fork = &all->forks_head[i + 1];
		philos[i].meal = &all->meals_head[i];
		philos[i].times_eaten = 0;
		philos[i].all_ptr = all;
		i++;
	}
	all->philos_head = philos;
	return (0);
}
