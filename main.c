#include "philo.h"

long get_time_ms(void)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	// get millisecond converting seconds to milliseconds
	long sec_to_milli = tv.tv_sec * 1000;
	// get millisecond converting microseconds to milliseconds
	long micro_to_milli = tv.tv_usec / 1000;

	// sum both to get total of milliseconds
	long total = sec_to_milli + micro_to_milli;
	return (total);
}

void	*philo_routine(void *arg)
{
	t_table	*args_struct;
	long start_time = get_time_ms();
	int	philo_id;
	args_struct = *static_args();
	pthread_mutex_lock(&args_struct->left_fork);
	pthread_mutex_lock(&args_struct->right_fork);

	philo_id = *(int *)arg;
	printf("%ldms [Philo id: %d] found.\n", get_time_ms() - start_time, philo_id);
	usleep(10000);
	printf("%ldms [Philo id: %d] waited and now is over.\n", get_time_ms() - start_time, philo_id);
	pthread_mutex_unlock(&args_struct->left_fork);
	pthread_mutex_unlock(&args_struct->right_fork);
	return (NULL);
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
	args_struct->philos = (ft_atoi)av[1];
	args_struct->time_to_die = (ft_atoi)av[2];
	args_struct->time_to_eat = (ft_atoi)av[3];
	args_struct->time_to_sleep = (ft_atoi)av[4];
	args_struct->left_fork = (ft_atoi)av[1];
	args_struct->right_fork = (ft_atoi)av[1];
	if (ac == 6)
		args_struct->num_times_to_eat = (ft_atoi)av[5];
	else
		args_struct->num_times_to_eat = -1;

	pthread_mutex_init(&left_fork, NULL);
	pthread_mutex_init(&right_fork, NULL);
	return (args_struct);
}

t_table	**static_args_struct(void)
{
	static t_table *args_struct;
	
	return (&args_struct);
}

int	main(int ac, char **av)
{
	t_table		*args_struct;
	//t_philo	*philo_struct;
	pthread_t philo_thread[6];
	int	philo_ids[6];
	//philo_struct = NULL;
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
	args_struct = init_table(ac, av);
	*static_args_struct() = args_struct;
	//store_philo(&philo_struct, av[1]);
	//print_test(&philo_struct);
	int i = 0;
	while (i < 6)
	{
		philo_ids[i] = i;
		pthread_create(&philo_thread[i], NULL, philo_routine, &philo_ids[i]);
		i++;
	}
	i = 0;
	while (i < 6)
	{
		pthread_join(philo_thread[i], NULL);
		i++;
	}
	pthread_mutex_destroy(&args_struct->left_fork);
	pthread_mutex_destroy(&args_struct->right_fork);
	return (0);
}
