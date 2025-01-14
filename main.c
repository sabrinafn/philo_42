/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabrifer <sabrifer@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 15:00:59 by sabrifer          #+#    #+#             */
/*   Updated: 2025/01/13 15:29:04 by sabrifer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
 * [1] num_philos - [2] time_to_die - [3] time_to_eat - [4] time_to_sleep
	- [5] times to eat
 *      5				400					200					200
 *  400 time to eat, then it eats at 0 millisecond
 *  then at 200 millisecond, at 400 millisecond
 *
 *		0 last meal time = 0
 *	200 current_time = 200. current_time - last_meal_time = 200
 *	400 current_time = 400. current_time - last_meal_time = 200
 *
 * [1] num_philos - [2] time_to_die - [3] time_to_eat - [4] time_to_sleep
	- [5] times to eat
 *      5				400					300					200
 *
 *		0 last meal time = 0
 *	300 current_time = 300. current_time - last_meal_time = 300
 *	400 current_time = 300. current_time - last_meal_time = 300
 * */

int		error_program_use(void);

void	mutex_printf(t_philo *philo, char *action)
{
	long int	start_time;
	long int	current_time;

	pthread_mutex_lock(&philo->table->mutex_printf);
	start_time = philo->table->start_time;
	current_time = timestamp_in_ms() - start_time;
//	pthread_mutex_lock(&philo->table->mutex_died);
//	if (philo->table->died == false)
//	{
		printf("%ld %d ", current_time, philo->philo_id);
		printf("%s\n", action);
//	}
//	pthread_mutex_unlock(&philo->table->mutex_died);
	pthread_mutex_unlock(&philo->table->mutex_printf);
}

void	take_forks_and_eat(t_philo *philo)
{
	pthread_mutex_t	*first;
	pthread_mutex_t	*second;

	// philo->last_meal_time = timestamp_in_ms() - philo->table->start_time;
	if ((philo->philo_id - 1) % 2 == 0)
	{
		first = philo->left_fork;
		second = philo->right_fork;
	}
	else
	{
		first = philo->right_fork;
		second = philo->left_fork;
	}
	pthread_mutex_lock(first);
	mutex_printf(philo, "has taken a LEFT fork");
	pthread_mutex_lock(second);
	mutex_printf(philo, "has taken a RIGHT fork");
	mutex_printf(philo, "is eating");
	// updates to a new value whenever philo eats
	pthread_mutex_lock(&philo->mutex_last_meal);
	philo->last_meal_time = timestamp_in_ms();
	pthread_mutex_unlock(&philo->mutex_last_meal);
	usleep(philo->table->time_to_eat * 1000);
	pthread_mutex_unlock(first);
	pthread_mutex_unlock(second);
	// increments +1 whenever philo eats
	philo->times_has_eaten++;
}

void	sleeping(t_philo *philo)
{
	mutex_printf(philo, "is sleeping");
	usleep(philo->table->time_to_sleep * 1000);
}

void	thinking(t_philo *philo)
{
	mutex_printf(philo, "is thinking");
}

void	die(t_philo *philo)
{
	mutex_printf(philo, "has died");
	pthread_mutex_lock(&philo->table->mutex_died);
	philo->table->died = true;
	pthread_mutex_unlock(&philo->table->mutex_died);
}

bool	end_routine(t_philo *philo)
{
	int	max;

	max = philo->table->max_times_to_eat;
	pthread_mutex_lock(&philo->table->mutex_died);
	if (philo->table->died == true)
	{
		pthread_mutex_unlock(&philo->table->mutex_died);
		return (true);
	}
	pthread_mutex_unlock(&philo->table->mutex_died);
	if (philo->times_has_eaten >= max)
		return (true);
	return (false);
}

void	*meal_routine(void *var)
{
	t_philo	*philo;

	philo = (t_philo *)var;
	while (!end_routine(philo))
	{
		take_forks_and_eat(philo);
		sleeping(philo);
		thinking(philo);
	}
	return (NULL);
}

void	track_meal_time(t_philo *philo)
{
	int	i;

	while (1)
	{
		i = 0;
		while (i < philo->table->num_philos)
		{
			pthread_mutex_lock(&philo[i].mutex_last_meal);
			if ((timestamp_in_ms() - philo[i].last_meal_time) > philo->table->time_to_die)
			{
				die(&philo[i]);
				pthread_mutex_unlock(&philo[i].mutex_last_meal);
				return ;
			}
			pthread_mutex_unlock(&philo[i].mutex_last_meal);
			i++;
		}
	}
	// eaten at 5, time now is 6. time to die = 6 = passed
	// eaten at 6, time now is 6. time to die = 6 = not passed
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
	//usleep(2000);
	track_meal_time(philo);
	i = 0;
	while (i < table->num_philos)
	{
		pthread_join(table->threads[i], NULL);
		i++;
	}
	pthread_mutex_destroy(&table->mutex_printf);
	pthread_mutex_destroy(&philo->mutex_last_meal);
	pthread_mutex_destroy(&table->mutex_died);
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
