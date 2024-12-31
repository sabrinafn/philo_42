#ifndef PHILO_H
#define PHILO_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

typedef struct s_philo
{
	int		philo;
	struct s_philo	*next;
}			t_philo;

typedef struct s_table
{
	int		philos;
	int		time_to_die;
	int		time_to_eat;
	int		time_to_sleep;
	int		num_times_to_eat;
	pthread_mutex_t	left_fork;
	pthread_mutex_t	right_fork;
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
