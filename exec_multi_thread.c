#include "philo.h"

int	ft_clean_up(t_all *all)
{
	pthread_mutex_destroy(&all->put_status);
	pthread_mutex_destroy(&all->count_full);
	pthread_mutex_destroy(&all->manage_flag);
	ft_destroy_mutex_until_index(all->nbr_of_philo, all->forks_head);
	ft_destroy_mutex_until_index(all->nbr_of_philo, all->meals_head);
	free(all->forks_head);
	free(all->meals_head);
	free(all->philos_head);
	free(all->threads_head);
	return (-1);
}

static int	ft_check_dead_or_alive(t_philo *philo)
{
	time_t	current_time;
	size_t	i;

	i = 0;
	while (i < philo->all_ptr->nbr_of_philo)
	{
		pthread_mutex_lock(&philo->meal[i]);
		current_time = ft_get_time();
		if (current_time - philo[i].start_time_of_last_meal > philo->all_ptr->time_to_die)
		{
			pthread_mutex_unlock(&philo->meal[i]);
			ft_put_status_message(DIED, &philo[i]);
			return (-1);
		}
		pthread_mutex_unlock(&philo->meal[i]);
		i++;
	}
	return (0);
}

static void	*ft_monitoring(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	usleep(10);//開始0秒died防止。
	while (1)
	{
		if (ft_check_dead_or_alive(philo) == -1)
			break;
		pthread_mutex_lock(&philo->all_ptr->count_full);
		if (philo->all_ptr->must_eat_count > 0 && philo->all_ptr->count_of_philo_full == philo->all_ptr->nbr_of_philo)
		{
			pthread_mutex_unlock(&philo->all_ptr->count_full);
			pthread_mutex_lock(&philo->all_ptr->manage_flag);
			philo->all_ptr->stop_flag = -1;
			pthread_mutex_unlock(&philo->all_ptr->manage_flag);
			break;
		}
		pthread_mutex_unlock(&philo->all_ptr->count_full);
	}
	return (NULL);
}

static int	ft_set_is_error_int(t_all *all)
{
	pthread_mutex_lock(&all->manage_flag);
	all->stop_flag = -1;
	pthread_mutex_unlock(&all->manage_flag);
	all->stop_flag = -1;
	return (0);
}

int	ft_exec_multi_thread(t_all *all)
{
	size_t		i;
	pthread_t	*philo_threads;
	pthread_t	monitoring_thread;

	philo_threads = (pthread_t *)malloc(sizeof(pthread_t) * all->nbr_of_philo);
	if (philo_threads == NULL)
		return (ft_clean_up(all));
	memset(philo_threads, 0, sizeof(pthread_t) * all->nbr_of_philo);// pthread_createが失敗した時用にmemset?
	all->threads_head = philo_threads;
	all->start_time_of_thread = ft_get_time();
	i = 0;
	while (i < all->nbr_of_philo)
	{
		if (pthread_create(&philo_threads[i], NULL, ft_simulation, (void *)&all->philos_head[i]) != 0)
			return (ft_set_is_error_int(all));//exitする訳ににもいかないので、正常終了処理へ進む。
		i++;
	}
	if (pthread_create(&monitoring_thread, NULL, ft_monitoring, (void *)all->philos_head) != 0)// こっちを先にcreateしたらどうなる？
		return (ft_set_is_error_int(all));
	pthread_join(monitoring_thread, NULL);
	return (0);
}
