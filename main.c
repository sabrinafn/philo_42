/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabrifer <sabrifer@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 15:00:59 by sabrifer          #+#    #+#             */
/*   Updated: 2025/01/09 15:09:17 by sabrifer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int		error_program_use(void);

void	*meal_routine(void *var)
{
	t_philo		*philo;
	long int	start_time;
	long int	current_time;

	philo = (t_philo *)var;
	start_time = philo->table->start_time;
	current_time = timestamp_in_ms() - start_time;
	// printf("start time in milliseconds: [%ld]\n", start_time);
	// usleep(5000);
	// printf("usleep(5000)\n");
	// printf("time since start time: [%ld]\n", current_time);
	pthread_mutex_lock(&philo->table->mutex_printf);
	printf("[%ld] - philo created [id = %d]\n", current_time, philo->philo_id);
	usleep(500000);
	pthread_mutex_unlock(&philo->table->mutex_printf);
	return (NULL);
}

void	start_meals(void)
{
	t_philo	*philos;
	t_table	*table;
	int		i;

	// array de structs
	philos = *static_philo_struct();
	table = *static_args_struct();
	i = 0;
	while (i < table->num_philos)
	{
		pthread_create(&table->threads[i], NULL, &meal_routine, &philos[i]);
		i++;
	}
	i = 0;
	while (i < table->num_philos)
	{
		pthread_join(table->threads[i], NULL);
		i++;
	}
	pthread_mutex_destroy(&table->mutex_printf);
}

int	main(int ac, char **av)
{
	if (ac != 5 && ac != 6)
		return (error_program_use());
	if (!is_valid_input(av))
		return (error_program_use());
	init_args_struct(ac, av);
	init_philos_struct(av);
	start_meals();
	return (0);
}

int	error_program_use(void)
{
	printf("PROGRAM USE: [number_of_philosophers] ");
	printf("[time_to_die] [time_to_eat] [time_to_sleep] ");
	printf("[number_of_times_each_philosopher_must_eat]");
	printf("(optional)\n");
	return (0);
}
