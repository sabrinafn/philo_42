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

bool	philo_died(t_philo *philo);

void	mutex_printf(t_philo *philo, char *action)
{
	long int	start_time;
	long int	current_time;

	pthread_mutex_lock(&philo->table->mutex_printf);
	start_time = philo->table->start_time;
	current_time = ft_time() - start_time;
	if (!philo_died(philo))
	{
		printf("%ld %d ", current_time, philo->philo_id);
		printf("%s\n", action);
	}
	pthread_mutex_unlock(&philo->table->mutex_printf);
}

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

void	free_philos(t_philo *philo, t_table *table)
{
	int	i;

	i = 0;
	pthread_mutex_destroy(&table->mutex_printf);
	pthread_mutex_destroy(&philo->mutex_last_meal);
	pthread_mutex_destroy(&table->mutex_died);
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
		track_routine(philo);
	}
	i = 0;
	while (i < table->num_philos)
	{
		pthread_join(table->threads[i], NULL);
		i++;
	}
	free_philos(philo, table);
}
