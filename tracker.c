/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   meal.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabrifer <sabrifer@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 15:40:08 by sabrifer          #+#    #+#             */
/*   Updated: 2025/01/17 15:40:23 by sabrifer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	die(t_philo *philo)
{
	mutex_printf(philo, "has died");
	pthread_mutex_lock(&philo->table->mutex_died);
	philo->table->died = true;
	pthread_mutex_unlock(&philo->table->mutex_died);
}

bool	all_have_eaten(t_philo *philo)
{
	int	i;
	int	counter;

	i = 0;
	counter = 0;
	if (philo->table->max_times_to_eat == -1)
		return (false);
	while (i < philo->table->num_philos)
	{
		if (philo[i].times_has_eaten >= philo->table->max_times_to_eat)
			counter++;
		i++;
	}
	if (counter == philo->table->num_philos)
		return (true);
	return (false);
}

bool	track_death(t_philo *philo)
{
	int	i;

	i = 0;
	while (i < philo->table->num_philos)
	{
		pthread_mutex_lock(&philo[i].mutex_last_meal);
		if ((ft_time() - philo[i].last_meal_time) > philo->table->time_to_die)
		{
			die(&philo[i]);
			pthread_mutex_unlock(&philo[i].mutex_last_meal);
			return (true);
		}
		pthread_mutex_unlock(&philo[i].mutex_last_meal);
		i++;
	}
	return (false);
}

bool	track_meals(t_philo *philo)
{
	int	i;

	i = 0;
	while (i < philo->table->num_philos)
	{
		pthread_mutex_lock(&philo[i].mutex_times_has_eaten);
		if (philo[i].times_has_eaten >= philo->table->max_times_to_eat)
		{
			if (all_have_eaten(philo))
			{
				pthread_mutex_lock(&philo->table->mutex_end_routine);
				philo->table->end_routine = true;
				pthread_mutex_unlock(&philo->table->mutex_end_routine);
				pthread_mutex_unlock(&philo[i].mutex_times_has_eaten);
				return (true);
			}
		}
		pthread_mutex_unlock(&philo[i].mutex_times_has_eaten);
		i++;
	}
	return (false);
}

void	track_routine(t_philo *philo)
{
	usleep(1000);
	while (1)
	{
		if (track_death(philo))
			return ;
		if (track_meals(philo))
			return ;
	}
}
