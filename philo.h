#ifndef PHILO_H
#define PHILO_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct s_philo
{
	int		philo;
	struct s_philo	*next;
}			t_philo;

#endif
