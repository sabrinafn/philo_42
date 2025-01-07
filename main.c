/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabrifer <sabrifer@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 15:00:59 by sabrifer          #+#    #+#             */
/*   Updated: 2025/01/07 15:01:00 by sabrifer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int		error_program_use(void);

void	*meal_routine(void *var)
{
	t_philo	*philo;

	philo = (t_philo *)var;

	//create mutex for printf
	//init mutex in function to init table
	// reference table with philo struct
	pthread_mutex_lock(&philo->table->mutex_printf);
//	printf("philo created [id = %d]\n", philo->philo_id);
//	usleep(10000);
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
//	printf("test 0\n");
	i = 0;
	while (i < table->num_philos)
	{
		pthread_create(&table->threads[i], NULL, &meal_routine, &philos[i]);
//		printf("hi\n");
		i++;
	}
	printf("hi\n");
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
