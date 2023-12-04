#include "philo.h"

static void	ft_taking_forks(t_philo *philo)
{
	if ((philo->philo_id) % 2 == 0 && philo->philo_id + 1 != philo->all_ptr->nbr_of_philo)// philoの人数が奇数の場合。
	{
		pthread_mutex_lock(philo->l_fork);
		ft_put_status_message(TAKE_FORK, philo);
		pthread_mutex_lock(philo->r_fork);
		ft_put_status_message(TAKE_FORK, philo);
	}
	else
	{
		usleep(50);//フォークを取るタイミングを少しだけずらす。
		pthread_mutex_lock(philo->r_fork);
		ft_put_status_message(TAKE_FORK, philo);
		pthread_mutex_lock(philo->l_fork);
		ft_put_status_message(TAKE_FORK, philo);
	}
}

static void	ft_eating(t_philo *philo)
{
	pthread_mutex_lock(philo->meal);
	philo->start_time_of_last_meal = ft_get_time();
	pthread_mutex_unlock(philo->meal);
	ft_put_status_message(EATING, philo);
	philo->times_eaten += 1;
	if (philo->times_eaten == philo->all_ptr->must_eat_count)
	{
		pthread_mutex_lock(&philo->all_ptr->count_full);
		philo->all_ptr->count_of_philo_full += 1;
		pthread_mutex_unlock(&philo->all_ptr->count_full);
	}
	ft_multi_usleep(philo->all_ptr->time_to_eat);
	pthread_mutex_unlock(philo->r_fork);
	pthread_mutex_unlock(philo->l_fork);
}

static void	ft_sleeping(t_philo *philo)
{
	ft_put_status_message(SLEEPING, philo);
	ft_multi_usleep(philo->all_ptr->time_to_sleep);
}

static void	ft_thinking(t_philo *philo)
{
	ft_put_status_message(THINKING, philo);
}

void	*ft_simulation(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	pthread_mutex_lock(philo->meal);
	philo->start_time_of_last_meal = ft_get_time();
	pthread_mutex_unlock(philo->meal);
	while (1)
	{
		pthread_mutex_lock(&philo->all_ptr->manage_flag);
		if (philo->all_ptr->stop_flag == -1)
		{
			pthread_mutex_unlock(&philo->all_ptr->manage_flag);
			break;
		}
		pthread_mutex_unlock(&philo->all_ptr->manage_flag);
		ft_taking_forks(philo);
		ft_eating(philo);//take_forkとeatはセットにすることでmutexがちゃんとunlockされる。
		ft_sleeping(philo);
		ft_thinking(philo);
	}
	return (NULL);
}
