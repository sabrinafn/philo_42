/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_structs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabrifer <sabrifer@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 15:01:17 by sabrifer          #+#    #+#             */
/*   Updated: 2025/01/17 17:56:29 by sabrifer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

t_table	*init_table_struct(int ac, char **av)
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
	table->end_routine = false;
	table->threads = malloc(sizeof(pthread_t) * ft_atoi(av[1]));
	table->forks = init_forks(ft_atoi(av[1]));
	pthread_mutex_init(&table->mutex_printf, NULL);
	pthread_mutex_init(&table->mutex_died, NULL);
	pthread_mutex_init(&table->mutex_end_routine, NULL);
	if (ac == 6)
		table->max_times_to_eat = ft_atoi(av[5]);
	else
		table->max_times_to_eat = -1;
	return (table);
}

t_philo	*init_philos_struct(char **av, t_table *table)
{
	int		i;
	int		num_philos;
	t_philo	*philos;

	i = 0;
	num_philos = ft_atoi(av[1]);
	philos = malloc(sizeof(t_philo) * num_philos);
	while (i < num_philos)
	{
		philos[i].philo_id = i + 1;
		philos[i].left_fork = &table->forks[i];
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
