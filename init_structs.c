/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_structs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabrifer <sabrifer@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 15:01:17 by sabrifer          #+#    #+#             */
/*   Updated: 2025/01/15 14:57:16 by sabrifer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// function to malloc forks.
// it needs to allocate memory so it can be a variable that can
// be used by all threads
pthread_mutex_t	*init_forks(int num)
{
	int				i;
	pthread_mutex_t	*forks;

	i = 0;
	forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * num);
	while (i < num)
	{
		pthread_mutex_init(&forks[i], NULL);
		i++;
	}
	return (forks);
}

// function that returns the time passing?
// it should start at 0 and then it will increase everytime it is called
long int	ft_time(void)
{
	struct timeval	current_time;
	long int		seconds;
	long int		micro_seconds;

	gettimeofday(&current_time, NULL);
	seconds = current_time.tv_sec * 1000;
	micro_seconds = current_time.tv_usec / 1000;
	return (seconds + micro_seconds);
}

// function to initialise structure of arguments received from the command line
// it also initialises threads and forks in the same structure
t_table	*init_table(int ac, char **av)
{
	t_table	*table;

	table = malloc(sizeof(t_table));
	if (!table)
	{
		printf("malloc error args_struct\n");
		return (NULL);
	}
	table->num_philos = ft_atoi(av[1]);
	table->time_to_die = ft_atoi(av[2]);
	table->time_to_eat = ft_atoi(av[3]);
	table->time_to_sleep = ft_atoi(av[4]);
	table->start_time = ft_time();
	table->died = false;
	table->full_philos = 0;
	table->end_routine = false;
	// init threads and forks based on number of philos
	table->threads = malloc(sizeof(pthread_t) * ft_atoi(av[1]));
	table->forks = init_forks(ft_atoi(av[1]));
	pthread_mutex_init(&table->mutex_printf, NULL);
	pthread_mutex_init(&table->mutex_died, NULL);
	pthread_mutex_init(&table->mutex_full_philos, NULL);
	pthread_mutex_init(&table->mutex_quit_table, NULL);
	// if there's an extra argument, then it is the number
	// of times each philo must eat:
	if (ac == 6)
		table->max_times_to_eat = ft_atoi(av[5]);
	else
		table->max_times_to_eat = -1;
	return (table);
}

t_philo	*init_philos(int num_philos, t_table *table)
{
	int		i;
	t_philo	*philos;

	i = 0;
	philos = malloc(sizeof(t_philo) * num_philos);
	while (i < num_philos)
	{
		philos[i].philo_id = i + 1;
		// reference the forks malloced in table structure
		// select which forks belong to each philo
		philos[i].left_fork = &table->forks[i];
		// (i + 1) % num_philos, it will give the value to wrap around
		// number of philos
		philos[i].right_fork = &table->forks[(i + 1) % num_philos];
		philos[i].last_meal_time = ft_time();
		philos[i].table = table;
		philos[i].times_has_eaten = 0;
		pthread_mutex_init(&philos[i].mutex_last_meal, NULL);
		pthread_mutex_init(&philos[i].mutex_times_has_eaten, NULL);
		i++;
	}
	return (philos);
}

t_table	*init_table_struct(int ac, char **av)
{
	t_table	*table;

	table = init_table(ac, av);
	return (table);
}

t_philo	*init_philos_struct(char **av, t_table *table)
{
	int		num_philo;
	t_philo	*philos_struct;

	num_philo = ft_atoi(av[1]);
	philos_struct = init_philos(num_philo, table);
	return (philos_struct);
}
