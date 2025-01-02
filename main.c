#include "philo.h"

t_table	**static_args_struct(void)
{
	static t_table *args_struct;
	
	return (&args_struct);
}

t_philo	**static_philo_struct(void)
{
	static t_philo *philos_struct;
	
	return (&philos_struct);
}

pthread_mutex_t	*init_forks(int num)
{
	int		i;
	pthread_mutex_t *forks;

	i = 0;
	forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * num);
	while (i < num)
	{
		pthread_mutex_init(&forks[i], NULL);
		i++;
	}
	return (forks);
}

t_table	*init_table(int ac, char **av)
{
	t_table	*args_struct;

	args_struct = malloc(sizeof(t_table *));
	if (!args_struct)
	{
		printf("malloc error args_struct\n");
		return (NULL);
	}
	args_struct->num_philos = ft_atoi(av[1]);
	args_struct->time_to_die = ft_atoi(av[2]);
	args_struct->time_to_eat = ft_atoi(av[3]);
	args_struct->time_to_sleep = ft_atoi(av[4]);
	args_struct->threads = malloc(sizeof(pthread_t) * ft_atoi(av[1]));
	args_struct->forks = init_forks(ft_atoi(av[1]));
	if (ac == 6)
		args_struct->max_times_to_eat = ft_atoi(av[5]);
	else
		args_struct->max_times_to_eat = -1;
	return (args_struct);
}

t_philo	*init_philos(int num_philos)
{
	int	i;
	t_philo	*philos;
	t_table	*args_struct;
	
	i = 0;
	philos = malloc(sizeof(t_philo) * num_philos);
	args_struct = *static_args_struct();

	while (i < num_philos)
	{
		philos[i].philo_id = i;
		philos[i].left_fork = &args_struct->forks[i];
		philos[i].right_fork = &args_struct->forks[(i + 1) % num_philos];
		philos[i].table = args_struct;
		i++;
	}
	return (philos);
}

void	init_args_struct(int ac, char **av)
{
	t_table		*args_struct;

	args_struct = init_table(ac, av);
	*static_args_struct() = args_struct;
}

void	init_philos_struct(char **av)
{
	int		num_philo;
	t_philo		*philos_struct;

	num_philo = ft_atoi(av[1]);
	philos_struct = init_philos(num_philo);
	*static_philo_struct() = philos_struct;
}

int	error_program_use(void)
{
	printf("PROGRAM USE: [number_of_philosophers] ");
	printf("[time_to_die] [time_to_eat] [time_to_sleep] ");
	printf("[number_of_times_each_philosopher_must_eat]");
	printf("(optional)\n");
	return (0);
}

void	philo_routine(void *arg)
{

	(void)arg;
	int	i = 0;
	printf("value of i = [%d]\n", i);
}

// need to create a routine

int	main(int ac, char **av)
{
	if (ac != 5 && ac != 6)
		return (error_program_use());
	if (!is_valid_input(av))
		return (error_program_use());
	init_args_struct(ac, av);
	init_philos_struct(av);
	
	return (0);
}
