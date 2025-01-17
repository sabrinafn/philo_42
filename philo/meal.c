/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   meal.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabrifer <sabrifer@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 15:40:08 by sabrifer          #+#    #+#             */
/*   Updated: 2025/01/17 17:47:51 by sabrifer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
