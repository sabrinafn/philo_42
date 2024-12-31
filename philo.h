#ifndef PHILO_H
#define PHILO_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

typedef struct s_philo
{
	int		philo_id;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	struct s_table	*table;
}			t_philo;

typedef struct s_table
{
	int		num_philos;
	int		time_to_die;
	int		time_to_eat;
	int		time_to_sleep;
	int		max_times_to_eat;
	pthread_t	*threads;
	pthread_mutex_t	*forks;
	struct s_philo	*philos;
}			t_table;

int     is_valid_input(char **args);
int     ft_isdigit(int c);
int     ft_atoi(const char *str);
int     is_integer(char **args);
int     is_positive(char **args);

// philo struct functions:
void    print_test(t_philo **philo_struct);
void    store_philo(t_philo **philos_struct, char *philos_str);
#endif
