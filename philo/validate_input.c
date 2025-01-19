/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_input.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabrifer <sabrifer@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 15:01:10 by sabrifer          #+#    #+#             */
/*   Updated: 2025/01/19 15:50:29 by sabrifer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_atoi(const char *str)
{
	int	i;
	int	sign;
	int	nbr;

	i = 0;
	sign = 1;
	nbr = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
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
	while (args[i] != NULL)
	{
		j = 0;
		while (args[i][j] != '\0')
		{
			if (!(args[i][j] >= '0' && args[i][j] <= '9'))
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

int	min_philo_quantity(char *philos)
{
	int	philo_i;

	philo_i = ft_atoi(philos);
	if (philo_i >= 1)
		return (1);
	return (0);
}

int	is_valid_input(char **args)
{
	if (!is_integer(args))
		return (0);
	if (!is_positive(args))
		return (0);
	if (!min_philo_quantity(args[1]))
		return (0);
	return (1);
}
