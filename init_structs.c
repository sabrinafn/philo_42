/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_structs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabrifer <sabrifer@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 15:01:17 by sabrifer          #+#    #+#             */
/*   Updated: 2025/01/09 15:09:28 by sabrifer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// static function to handle table struct (just in case i need it).
t_table	**static_args_struct(void)
{
	static t_table	*args_struct;

	return (&args_struct);
}

// static function to handle philos struct (just in case i need it).
t_philo	**static_philo_struct(void)
{
	static t_philo	*philos_struct;

	return (&philos_struct);
}

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
long int	timestamp_in_ms(void)
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
	t_table	*args_struct;

	args_struct = malloc(sizeof(t_table));
	if (!args_struct)
	{
		printf("malloc error args_struct\n");
		return (NULL);
	}
	args_struct->num_philos = ft_atoi(av[1]);
	args_struct->time_to_die = ft_atoi(av[2]);
	args_struct->time_to_eat = ft_atoi(av[3]);
	args_struct->time_to_sleep = ft_atoi(av[4]);
	args_struct->start_time = timestamp_in_ms();
//	args_struct->last_meal = 0;
	// init threads and forks based on number of philos
	args_struct->threads = malloc(sizeof(pthread_t) * ft_atoi(av[1]));
	args_struct->forks = init_forks(ft_atoi(av[1]));
	if (pthread_mutex_init(&args_struct->mutex_printf, NULL) != 0)
	{
		printf("mutex init failed\n");
		return (NULL);
	}
	// if there's an extra argument, then it is the number
	// of times each philo must eat
	if (ac == 6)
		args_struct->max_times_to_eat = ft_atoi(av[5]);
	else
		args_struct->max_times_to_eat = -1; // idk?
	return (args_struct);
}

t_philo	*init_philos(int num_philos)
{
	int		i;
	t_philo	*philos;
	t_table	*args_struct;

	i = 0;
	philos = malloc(sizeof(t_philo) * num_philos);
	args_struct = *static_args_struct();
	while (i < num_philos)
	{
		philos[i].philo_id = i;
		// reference the forks malloced in table structure
		// select which forks belong to each philo
		philos[i].left_fork = &args_struct->forks[i];
		// (i + 1) % num_philos, it will give the value to wrap around
		// number of philos
		philos[i].right_fork = &args_struct->forks[(i + 1) % num_philos];
		philos[i].table = args_struct;
		i++;
	}
	return (philos);
}

void	init_args_struct(int ac, char **av)
{
	t_table *args_struct; // declaring table struct
	args_struct = init_table(ac, av);
	// initializing structure with arguments
	*static_args_struct() = args_struct;
	// store structure in static function :)
}

void	init_philos_struct(char **av)
{
	int	num_philo;

	t_philo *philos_struct; // declaring philo struct
	num_philo = ft_atoi(av[1]);
	philos_struct = init_philos(num_philo);
	*static_philo_struct() = philos_struct;
}
