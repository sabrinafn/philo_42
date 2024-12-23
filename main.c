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
	{
		printf("!is_integer\n");
		return (0);
	}
	if (!is_positive(args))
	{
		printf("!is_positive\n");
		return (0);
	}
	return (1);
}

int	main(int ac, char **av)
{
	if (ac != 5 && ac != 6)
	{
		printf("PROGRAM USE: [number_of_philosophers] [time_to_die] [time_to_eat] [time_to_sleep] [number_of_times_each_philosopher_must_eat](optional)\n");
		return (0);
	}
	if (is_valid_input(av))
		printf("CORRECT ARGS\n");
	else if (!is_valid_input(av))
		printf("INCORRECT ARGS\n");

	return (0);
}
