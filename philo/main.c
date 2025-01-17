/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabrifer <sabrifer@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 15:00:59 by sabrifer          #+#    #+#             */
/*   Updated: 2025/01/15 14:50:25 by sabrifer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	error_program_use(void)
{
	printf("PROGRAM USE: [number_of_philosophers] ");
	printf("[time_to_die] [time_to_eat] [time_to_sleep] ");
	printf("[number_of_times_each_philosopher_must_eat]");
	printf("(optional)\n");
	return (0);
}

int	main(int ac, char **av)
{
	t_philo	*philo;
	t_table	*table;

	if (ac != 5 && ac != 6)
		return (error_program_use());
	if (!is_valid_input(av))
		return (error_program_use());
	table = init_table_struct(ac, av);
	philo = init_philos_struct(av, table);
	start_meals(philo, table);
	return (0);
}
