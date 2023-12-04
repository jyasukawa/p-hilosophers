#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <stdbool.h>
# include <limits.h>
# include <pthread.h>
# include <string.h>
# include <sys/time.h>

typedef enum	s_type 
{
	TAKE_FORK,
	EATING,
	SLEEPING,
	THINKING,
	DIED,
}	t_type;

struct	s_all;

typedef struct	s_philo
{
	size_t					philo_id;
	pthread_mutex_t	*l_fork;
	pthread_mutex_t	*r_fork;
	pthread_mutex_t	*meal;
	time_t					start_time_of_last_meal;
	size_t					times_eaten;
	struct s_all		*all_ptr;
}	t_philo;

typedef struct		s_all
{
	size_t					nbr_of_philo;
	int							time_to_die;
	int							time_to_eat;
	int							time_to_sleep;
	size_t					must_eat_count;

	pthread_mutex_t	put_status;

	pthread_mutex_t	count_full;
	size_t					count_of_philo_full;

	pthread_mutex_t	manage_flag;
	int							stop_flag;

	pthread_mutex_t	*meals_head;
	pthread_mutex_t	*forks_head;
	t_philo					*philos_head;
	pthread_t				*threads_head;
	time_t					start_time_of_thread;
}	t_all;

// philo_utils.c
void			ft_multi_usleep(int ms);
uint64_t	ft_get_time(void);
void			ft_destroy_mutex_until_index(size_t index, pthread_mutex_t	*forks);
int				ft_put_status_message(t_type type, t_philo *philo);

// initialize.c
int		ft_init_mutexes(t_all *all);
int		ft_init_philos(t_all *all);

// check_args.c
int			ft_check_args(int argc, char **argv);
ssize_t	ft_str_to_nbr(char *str);

// exec_multi_thread.c
int	ft_exec_multi_thread(t_all *all);
int	ft_clean_up(t_all *all);

// simulation.c
void	*ft_simulation(void *arg);

#endif