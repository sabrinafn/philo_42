/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabrifer <sabrifer@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 15:00:59 by sabrifer          #+#    #+#             */
/*   Updated: 2025/01/09 17:38:27 by sabrifer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int		error_program_use(void);

void	take_forks(t_philo *philo)
{
	long int	start_time;
	long int	current_time;

	start_time = philo->table->start_time;
	current_time = timestamp_in_ms() - start_time;
	// testing with forks
	pthread_mutex_lock(philo->left_fork);
	pthread_mutex_lock(philo->right_fork);
	pthread_mutex_lock(&philo->table->mutex_printf);
	printf("%ld %d has taken a fork\n", current_time, philo->philo_id);
	printf("%ld %d has taken a fork\n", current_time, philo->philo_id);
	pthread_mutex_unlock(&philo->table->mutex_printf);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

void	eating(t_philo *philo)
{
	long int	start_time;
	long int	current_time;

	start_time = philo->table->start_time;
	// testing mutex with first printf EATING
	pthread_mutex_lock(&philo->table->mutex_printf);
	current_time = timestamp_in_ms() - start_time;
	printf("%ld %d is eating\n", current_time, philo->philo_id);
	pthread_mutex_unlock(&philo->table->mutex_printf);
	usleep(philo->table->time_to_eat);
}

void	sleeping(t_philo *philo)
{
	long int	start_time;
	long int	current_time;

	start_time = philo->table->start_time;
	// testing mutex with first printf SLEEPING
	pthread_mutex_lock(&philo->table->mutex_printf);
	current_time = timestamp_in_ms() - start_time;
	printf("%ld %d is sleeping\n", current_time, philo->philo_id);
	pthread_mutex_unlock(&philo->table->mutex_printf);
	usleep(philo->table->time_to_sleep);
}

void	thinking(t_philo *philo)
{
	long int	start_time;
	long int	current_time;

	start_time = philo->table->start_time;
	// testing mutex with first printf THIKING
	pthread_mutex_lock(&philo->table->mutex_printf);
	current_time = timestamp_in_ms() - start_time;
	printf("%ld %d is thinking\n", current_time, philo->philo_id);
	pthread_mutex_unlock(&philo->table->mutex_printf);
	usleep(philo->table->time_to_sleep);
}

void	*meal_routine(void *var)
{
	t_philo		*philo;
	long int	start_time;
	long int	current_time;

	philo = (t_philo *)var;
	start_time = philo->table->start_time;
	current_time = timestamp_in_ms() - start_time;
	take_forks(philo);
	eating(philo);
	sleeping(philo);
	thinking(philo);
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
