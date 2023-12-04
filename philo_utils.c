#include "philo.h"

void	ft_multi_usleep(int ms)
{
	uint64_t	time;

	time = ft_get_time();
	usleep(ms * 900);
	while (ft_get_time() < time + ms)//actionが終わる予定時刻になるまでusleepを細切れに実行。
		usleep(ms * 3);
}

uint64_t	ft_get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	ft_destroy_mutex_until_index(size_t index, pthread_mutex_t	*forks)
{
	size_t	i;

	i = 0;
	while (i < index)
	{
		pthread_mutex_destroy(&forks[i]);
		i++;
	}
}

static void	ft_manage_flag(t_philo *philo)
{
	pthread_mutex_lock(&philo->all_ptr->manage_flag);
	philo->all_ptr->stop_flag = -1;
	pthread_mutex_unlock(&philo->all_ptr->manage_flag);
}

int	ft_put_status_message(t_type type, t_philo *philo)
{
	time_t	time;

	pthread_mutex_lock(&philo->all_ptr->manage_flag);
	if (philo->all_ptr->stop_flag == -1)
		return (pthread_mutex_unlock(&philo->all_ptr->manage_flag));
	pthread_mutex_unlock(&philo->all_ptr->manage_flag);
	pthread_mutex_lock(&philo->all_ptr->put_status);
	time = ft_get_time() - philo->all_ptr->start_time_of_thread;
	if (type == TAKE_FORK)
		printf("%ld %lu has taken a fork\n", time, philo->philo_id + 1);
	else if (type == EATING)
		printf("%ld %lu is eating\n", time, philo->philo_id + 1);
	else if (type == SLEEPING)
		printf("%ld %lu is sleeping\n", time, philo->philo_id + 1);
	else if (type == THINKING)
		printf("%ld %lu is thinking\n", time, philo->philo_id + 1);
	else if (type == DIED)
	{
		ft_manage_flag(philo);
		printf("%ld %lu died\n", time, philo->philo_id + 1);
		usleep(500000);//以降出力が出ないように0.5秒停止させる。
	}
	pthread_mutex_unlock(&philo->all_ptr->put_status);
	return (0);
}
