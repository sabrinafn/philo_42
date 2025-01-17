/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabrifer <sabrifer@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 15:40:08 by sabrifer          #+#    #+#             */
/*   Updated: 2025/01/17 17:30:54 by sabrifer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

bool	routine_over(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->mutex_end_routine);
	if (philo->table->end_routine == true)
	{
		pthread_mutex_unlock(&philo->table->mutex_end_routine);
		return (true);
	}
	pthread_mutex_unlock(&philo->table->mutex_end_routine);
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
		if (routine_over(philo))
			break ;
		take_forks_and_eat(philo);
		sleeping(philo);
		thinking(philo);
	}
	return (NULL);
}

void	*one_philo_routine(void *var)
{
	t_philo	*philo;

	philo = (t_philo *)var;
	pthread_mutex_lock(philo->left_fork);
	mutex_printf(philo, "has taken a fork");
	pthread_mutex_unlock(philo->left_fork);
	usleep(philo->table->time_to_die * 1000);
	mutex_printf(philo, "has died");
	return (NULL);
}
