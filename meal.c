/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   meal.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabrifer <sabrifer@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 15:00:59 by sabrifer          #+#    #+#             */
/*   Updated: 2025/01/15 15:09:27 by sabrifer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	mutex_printf(t_philo *philo, char *action)
{
	long int	start_time;
	long int	current_time;

	pthread_mutex_lock(&philo->table->mutex_printf);
	start_time = philo->table->start_time;
	current_time = timestamp_in_ms() - start_time;
	pthread_mutex_lock(&philo->table->mutex_died);
	pthread_mutex_lock(&philo->table->mutex_quit_table);
	if (philo->table->died == false || philo->table->quit_table == false)
	{
		printf("%ld %d ", current_time, philo->philo_id);
		printf("%s\n", action);
	}
	pthread_mutex_unlock(&philo->table->mutex_quit_table);
	pthread_mutex_unlock(&philo->table->mutex_died);
	pthread_mutex_unlock(&philo->table->mutex_printf);
}

void	update_meals(t_philo *philo)
{
	// updates to a new value whenever philo eats
	pthread_mutex_lock(&philo->mutex_last_meal);
	philo->last_meal_time = timestamp_in_ms();
	// increments +1 whenever philo eats
	if (philo->table->max_times_to_eat != -1)
	{
		philo->times_has_eaten++;
		pthread_mutex_lock(&philo->table->mutex_full_philos);
		if (philo->times_has_eaten == philo->table->max_times_to_eat)
			philo->table->full_philos++;
		// increments whenever a philo ate all the times it should
		pthread_mutex_unlock(&philo->table->mutex_full_philos);
	}
	pthread_mutex_unlock(&philo->mutex_last_meal);
}

void	take_forks_and_eat(t_philo *philo)
{
	pthread_mutex_t	*first;
	pthread_mutex_t	*second;

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
	mutex_printf(philo, "has taken a fork (1)");
	pthread_mutex_lock(second);
	mutex_printf(philo, "has taken a fork (2)");
	mutex_printf(philo, "is eating");
	update_meals(philo);
	usleep(philo->table->time_to_eat * 1000);
	pthread_mutex_unlock(second);
	pthread_mutex_unlock(first);
}

void	sleeping(t_philo *philo)
{
	mutex_printf(philo, "is sleeping");
	usleep(philo->table->time_to_sleep * 1000);
}

void	thinking(t_philo *philo)
{
	mutex_printf(philo, "is thinking");
	usleep(1000);
}

void	die(t_philo *philo)
{
	mutex_printf(philo, "has died");
	pthread_mutex_lock(&philo->table->mutex_died);
	philo->table->died = true;
	pthread_mutex_unlock(&philo->table->mutex_died);
}

bool	philo_died(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->mutex_died);
	if (philo->table->died == true)
	{
		pthread_mutex_unlock(&philo->table->mutex_died);
		return (true);
	}
	pthread_mutex_unlock(&philo->table->mutex_died);
	return (false);
}

bool	philo_are_full(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->mutex_full_philos);
	if (philo->table->full_philos == philo->table->num_philos)
	{
		pthread_mutex_unlock(&philo->table->mutex_full_philos);
		return (true);
	}
	pthread_mutex_unlock(&philo->table->mutex_full_philos);
	return (false);
}

bool	philo_ate_max_times(t_philo *philo)
{
	pthread_mutex_lock(&philo->mutex_times_has_eaten);
	if (philo->times_has_eaten == philo->table->max_times_to_eat)
	{
		pthread_mutex_unlock(&philo->mutex_times_has_eaten);
		return (true);
	}
	pthread_mutex_unlock(&philo->mutex_times_has_eaten);
	return (false);
}

void	*meal_routine(void *var)
{
	t_philo	*philo;

	philo = (t_philo *)var;
	while (1)
	{
		if (philo_died(philo))
			break ;
		if (philo_ate_max_times(philo))
			break ;
		pthread_mutex_lock(&philo->table->mutex_quit_table);
		if (philo->table->quit_table == true)
		{
			pthread_mutex_unlock(&philo->table->mutex_quit_table);
			break ;
		}
		pthread_mutex_unlock(&philo->table->mutex_quit_table);
		take_forks_and_eat(philo);
		sleeping(philo);
		pthread_mutex_unlock(&philo->table->mutex_quit_table);
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
				printf("timestamp_in_ms() - philo[i].last_meal_time = [%ld]\n", timestamp_in_ms() - philo[i].last_meal_time);
				if (philo[i].times_has_eaten < philo->table->max_times_to_eat)
				{
					printf("timestamp_in_ms() - philo[i].last_meal_time = [%ld]\n", timestamp_in_ms() - philo[i].last_meal_time);
					die(&philo[i]);
					pthread_mutex_unlock(&philo[i].mutex_last_meal);
					return ;
				}
			}
			pthread_mutex_unlock(&philo[i].mutex_last_meal);
			if (philo_are_full(philo))
			{
				pthread_mutex_lock(&philo->table->mutex_quit_table);
				philo->table->quit_table = true;
				printf("philo->table->quit_table = [%d]\n", philo->table->quit_table);
				pthread_mutex_unlock(&philo->table->mutex_quit_table);
				return ;
			}
			i++;
		}
		usleep(1000);
	}
	// eaten at 5, time now is 6. time to die = 6 = passed
	// eaten at 6, time now is 6. time to die = 6 = not passed
}

void	free_philos(t_philo *philo, t_table *table)
{
	int	i;

	i = 0;
	pthread_mutex_destroy(&table->mutex_printf);
	pthread_mutex_destroy(&philo->mutex_last_meal);
	pthread_mutex_destroy(&table->mutex_died);
	pthread_mutex_destroy(&table->mutex_full_philos);
	pthread_mutex_destroy(&table->mutex_quit_table);
	i = 0;
	while (i < table->num_philos)
	{
		pthread_mutex_destroy(&table->forks[i]);
		i++;
	}
	free(table->forks);
	free(table->threads);
	free(table);
	free(philo);
}

void	*one_philo_routine(void *var)
{
	t_philo	*philo;

	philo = (t_philo *)var;
	pthread_mutex_lock(philo->left_fork);
	mutex_printf(philo, "has taken a fork (1)");
	pthread_mutex_unlock(philo->left_fork);
	mutex_printf(philo, "has died");
	
	return (NULL);
}

void	start_meals(t_philo *philo, t_table *table)
{
	int	i;

	// array de structs
	i = 0;
	if (philo->table->num_philos == 1)
		pthread_create(&table->threads[i], NULL, &one_philo_routine, &philo[i]);
	else
	{
		while (i < table->num_philos)
		{
			pthread_create(&table->threads[i], NULL, &meal_routine, &philo[i]);
			i++;
		}
		track_meal_time(philo);
	}
	i = 0;
	while (i < table->num_philos)
	{
		pthread_join(table->threads[i], NULL);
		i++;
	}
	free_philos(philo, table);
}