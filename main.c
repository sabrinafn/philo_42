#include "philo.h"

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

static int	is_white_space(char c)
{
	if ((c >= 9 && c <= 13) || c == 32)
		return (1);
	return (0);
}

int	ft_atoi(const char *str)
{
	int	i;
	int	sign;
	int	nbr;

	i = 0;
	sign = 1;
	nbr = 0;
	while (is_white_space(str[i]) == 1)
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (ft_isdigit(str[i]) == 1)
	{
		nbr = nbr * 10 + (str[i] - '0');
		i++;
	}
	return (nbr * sign);
}

int	is_integer(char **args)
{
	int	i;
	int	j;

	i = 1;
	j = 0;
	
	while (args[i])
	{
		while (args[i][j])
		{
			if (!ft_isdigit(args[i][j]))
				return (0);
			j++;
		}
		i++;
	}
	return (1);
}

int	is_positive(char **args)
{
	int	i;

	i = 1;
	while (args[i])
	{
		if (ft_atoi(args[i]) <= 0)
			return (0);
		i++;
	}
	return (1);
}

int	is_valid_input(char **args)
{
	if (!is_integer(args))
		return (0);
	if (!is_positive(args))
		return (0);
	return (1);
}

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

void	*philo_routine(void *arg)
{
	int	philo_id;

	philo_id = *(int *)arg;
	printf("Philo [id: %d] found.\n", philo_id);
	usleep(1000000);
	printf("Philo [id: %d] waited and now is over.\n", philo_id);
	return (NULL);
}

int	main(int ac, char **av)
{
	t_philo	*philo_struct;
	pthread_t philo_thread[ac - 1];

	philo_struct = NULL;
	if (ac != 5 && ac != 6)
	{
		printf("PROGRAM USE: [number_of_philosophers] [time_to_die] [time_to_eat] [time_to_sleep] [number_of_times_each_philosopher_must_eat](optional)\n");
		return (0);
	}
	if (!is_valid_input(av))
	{
		printf("INCORRECT ARGS\n");
		return (0);
	}
	store_philo(&philo_struct, av[1]);
	//print_test(&philo_struct);
	int i = 0;
	while (i < ac - 1)
	{
		pthread_create(&philo_thread[i], NULL, philo_routine, &i);
		i++;
	}
	i = 0;
	while (i < ac - 1)
	{
		pthread_join(philo_thread[i], NULL);
		i++;
	}
	return (0);
}
