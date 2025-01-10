/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabrifer <sabrifer@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 15:01:26 by sabrifer          #+#    #+#             */
/*   Updated: 2025/01/07 15:01:28 by sabrifer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h> // pthread_create, pthread_join, pthread_mutex
# include <stdio.h> // printf
# include <stdlib.h> // malloc
# include <sys/time.h> // gettimeofday
# include <unistd.h> // 

typedef struct s_philo
{
	int				philo_id;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	struct s_table	*table;
}					t_philo;

typedef struct s_table
{
	int				num_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				max_times_to_eat;
	long int		start_time;
//	int				last_meal;
	pthread_t		*threads;
	pthread_mutex_t	*forks;
	pthread_mutex_t	mutex_printf;
	pthread_mutex_t	mutex_while;
	struct s_philo	*philos;
}					t_table;

int					is_valid_input(char **args);
int					ft_isdigit(int c);
int					ft_atoi(const char *str);
int					is_integer(char **args);
int					is_positive(char **args);

// init_philo_structs.c
t_table				**static_args_struct(void);
t_philo				**static_philo_struct(void);
pthread_mutex_t		*init_forks(int num);
t_table				*init_table(int ac, char **av);
t_philo				*init_philos(int num_philos);
void				init_args_struct(int ac, char **av);
void				init_philos_struct(char **av);
long int	timestamp_in_ms(void);

#endif
