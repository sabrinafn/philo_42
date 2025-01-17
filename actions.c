/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabrifer <sabrifer@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 15:40:08 by sabrifer          #+#    #+#             */
/*   Updated: 2025/01/17 17:40:39 by sabrifer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	update_meals(t_philo *philo)
{
	pthread_mutex_lock(&philo->mutex_last_meal);
	philo->last_meal_time = ft_time();
	if (philo->table->max_times_to_eat != -1)
		philo->times_has_eaten++;
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
	mutex_printf(philo, "has taken a fork");
	pthread_mutex_lock(second);
	mutex_printf(philo, "has taken a fork");
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
