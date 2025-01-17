/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabrifer <sabrifer@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 15:01:26 by sabrifer          #+#    #+#             */
/*   Updated: 2025/01/17 17:56:22 by sabrifer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_philo
{
	int				philo_id;
	int				times_has_eaten;
	long int		last_meal_time;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	mutex_last_meal;
	pthread_mutex_t	mutex_times_has_eaten;
	struct s_table	*table;
}					t_philo;

typedef struct s_table
{
	bool			died;
	bool			end_routine;
	int				num_philos;
	int				max_times_to_eat;
	long int		time_to_die;
	long int		time_to_eat;
	long int		time_to_sleep;
	long int		start_time;
	pthread_t		*threads;
	pthread_mutex_t	*forks;
	pthread_mutex_t	mutex_printf;
	pthread_mutex_t	mutex_died;
	pthread_mutex_t	mutex_end_routine;
	struct s_philo	*philos;
}					t_table;

// validate_input.c
int					ft_atoi(const char *str);
int					is_integer(char **args);
int					is_positive(char **args);
int					min_philo_quantity(char *philos);
int					is_valid_input(char **args);

// init_philo_structs.c
pthread_mutex_t		*init_forks(int num);
long int			ft_time(void);
t_table				*init_table_struct(int ac, char **av);
t_philo				*init_philos_struct(char **av, t_table *table);

// tracker.c
void				die(t_philo *philo);
bool				all_have_eaten(t_philo *philo);
bool				track_death(t_philo *philo);
bool				track_meals(t_philo *philo);
void				track_routine(t_philo *philo);

// routine.c
bool				philo_died(t_philo *philo);
bool				routine_over(t_philo *philo);
void				*meal_routine(void *var);
void				*one_philo_routine(void *var);

// actions.c
void				update_meals(t_philo *philo);
void				take_forks_and_eat(t_philo *philo);
void				sleeping(t_philo *philo);
void				thinking(t_philo *philo);

// meal.c
void				mutex_printf(t_philo *philo, char *action);
void				free_philos(t_philo *philo, t_table *table);
void				start_meals(t_philo *philo, t_table *table);

#endif
