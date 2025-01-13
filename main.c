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

void	mutex_printf(t_philo *philo, char *action)
{
	long int		start_time;
	long int		current_time;
	
	pthread_mutex_lock(&philo->table->mutex_printf);
	start_time = philo->table->start_time;
	current_time = timestamp_in_ms() - start_time;
	printf("%ld %d ", current_time, philo->philo_id);
	printf("%s\n", action);
	pthread_mutex_unlock(&philo->table->mutex_printf);
}

void	take_forks(t_philo *philo)
{
	long int		start_time;
	long int		current_time;
	pthread_mutex_t		*first;
	pthread_mutex_t		*second;

	start_time = philo->table->start_time;
	current_time = timestamp_in_ms() - start_time;
	// testing with forks
	if (philo->philo_id % 2 == 0)
	{
		first = philo->left_fork;
		second = philo->right_fork;
	}
	else
	{
		second = philo->right_fork;
		first = philo->left_fork;
	}
	printf("hello\n");
	pthread_mutex_lock(first);
	mutex_printf(philo, "has taken a fork");
	pthread_mutex_lock(second);
	mutex_printf(philo, "has taken a fork");
	printf("hi\n");
}

void	eating(t_philo *philo)
{
	take_forks(philo);
	mutex_printf(philo, "is eating");
	usleep(philo->table->time_to_eat);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

void	sleeping(t_philo *philo)
	{
	mutex_printf(philo, "is sleeping");
	usleep(philo->table->time_to_sleep);
}

void	thinking(t_philo *philo)
{
	mutex_printf(philo, "is thinking");
	usleep(philo->table->time_to_sleep);
}
/*
void	die(t_philo *philo)
{
	long int	start_time;
	long int	current_time;

	start_time = philo->table->start_time;
	pthread_mutex_lock(&philo->table->mutex_printf);
	current_time = timestamp_in_ms() - start_time;
	printf("%ld %d died\n", current_time, philo->philo_id);
	pthread_mutex_unlock(&philo->table->mutex_printf);
	usleep(philo->table->time_to_sleep);
}
*/
void	*meal_routine(void *var)
{
	t_philo		*philo;
	//long int	start_time;

	philo = (t_philo *)var;
	//start_time = philo->table->start_time;
	int	i = 0;
	while (i < 5)
	{
		//pthread_mutex_lock(&philo->table->mutex_while);
		eating(philo);
		sleeping(philo);
		thinking(philo);
		i++;
		//pthread_mutex_unlock(&philo->table->mutex_while);
	}
	return (NULL);
}

void	start_meals(void)
{
	t_philo	*philo;
	t_table	*table;
	int		i;

	// array de structs
	philo = *static_philo_struct();
	table = *static_args_struct();
	i = 0;
	while (i < table->num_philos)
	{
		pthread_create(&table->threads[i], NULL, &meal_routine, &philo[i]);
		i++;
	}
	// create a function to track last meal time to calculate:
	// timestamp_in_ms() - last_meal_time > time_to_die
	sleep(2);
	i = 0;
	while (i < table->num_philos)
	{
		pthread_join(table->threads[i], NULL);
		i++;
	}
	pthread_mutex_destroy(&table->mutex_printf);
	pthread_mutex_destroy(&table->mutex_while);
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
