/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabrifer <sabrifer@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 15:01:26 by sabrifer          #+#    #+#             */
/*   Updated: 2025/01/15 11:48:46 by sabrifer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>  // pthread_create, pthread_join, pthread_mutex
# include <stdbool.h>  // bool var
# include <stdio.h>    // printf
# include <stdlib.h>   // malloc
# include <sys/time.h> // gettimeofday
# include <unistd.h>   //

typedef struct s_philo
{
	int				philo_id;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	long int		last_meal_time;
	pthread_mutex_t	mutex_last_meal;
	pthread_mutex_t	mutex_times_has_eaten;
	int				times_has_eaten;
	struct s_table	*table;
}					t_philo;

typedef struct s_table
{
	int				num_philos;
	long int		time_to_die;
	long int		time_to_eat;
	long int		time_to_sleep;
	int				max_times_to_eat;
	long int		start_time;
	bool			died;
	bool			end_routine;
	pthread_t		*threads;
	pthread_mutex_t	*forks;
	pthread_mutex_t	mutex_printf;
	pthread_mutex_t	mutex_died;
	pthread_mutex_t	mutex_end_routine;
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
t_philo				*init_philos(int num_philos, t_table *table);
t_table				*init_table_struct(int ac, char **av);
t_philo				*init_philos_struct(char **av, t_table *table);
long int			ft_time(void);

// tracker.c
void	die(t_philo *philo);
bool	all_have_eaten(t_philo *philo);
bool	track_death(t_philo *philo);
bool	track_meals(t_philo *philo);
void	track_routine(t_philo *philo);

// meal file
void				start_meals(t_philo *philo, t_table *table);


#endif
