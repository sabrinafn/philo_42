#include "philo.h"

void	store_philo(t_philo **philos_struct, char *philos_str)
{
	int	philos_amount;
	int	i;
	t_philo	*new_philo;
	t_philo	*last_philo;

	i = 0;
	philos_amount = ft_atoi(philos_str);
	new_philo = NULL;
	last_philo = *philos_struct;
	while (i < philos_amount)
	{
		new_philo = malloc(sizeof(t_philo *));
		new_philo -> philo = i + 1;
		new_philo -> next = NULL;
		if (*philos_struct == NULL)
			*philos_struct = new_philo;
		else
		{
			last_philo = *philos_struct;
			while (last_philo->next)
				last_philo = last_philo -> next;
			last_philo -> next = new_philo;
		}
		i++;
	}
}

void	print_test(t_philo **philo_struct)
{
	t_philo	*philos;

	philos = *philo_struct;
	while (philos)
	{
		printf("Philosopher %d created.\n", philos->philo);
		philos = philos->next;
	}			
}
