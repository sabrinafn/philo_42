#include "philo.h"

pthread_mutex_t mutex;

long get_time_ms(void)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	long sec_to_milli = tv.tv_sec * 1000;
	long micro_to_milli = tv.tv_usec / 1000;

	long total = sec_to_milli + micro_to_milli;
	return (total);
}

void	*philo_routine(void *arg)
{
	long start_time = get_time_ms();
	int	philo_id;
	pthread_mutex_lock(&mutex);

	philo_id = *(int *)arg;
	printf("%ldms [Philo id: %d] found.\n", get_time_ms() - start_time, philo_id);
	printf("%ldms [Philo id: %d] waited and now is over.\n", get_time_ms() - start_time, philo_id);
	pthread_mutex_unlock(&mutex);
	return (NULL);
}

int	main(int ac, char **av)
{
	//t_philo	*philo_struct;
	pthread_t philo_thread[6];
	int	philo_ids[6];
	pthread_mutex_init(&mutex, NULL);
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
	pthread_mutex_destroy(&mutex);
	return (0);
}
