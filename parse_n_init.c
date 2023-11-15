/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_n_init.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfrants <vfrants@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 21:02:53 by vfrants           #+#    #+#             */
/*   Updated: 2023/11/14 20:59:17 by vfrants          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	ft_atoi(const char *str)
{
	int	number;

	number = 0;
	while (*str >= '0' && *str <= '9')
	{
		number = number * 10 + *str - '0';
		str++;
	}
	return (number);
}

static int	is_invalid_number(char *str)
{
	long int	res;

	res = 0;
	if (*str < '0' || *str > '9')
		return (1);
	while (*str)
	{
		if (*str < '0' || *str > '9')
			return (1);
		res = (res * 10) + (*str - '0');
		str++;
		if (res > 2147483647)
			return (1);
	}
	return (0);
}

int	parse_args(int argc, char **argv, t_data *data)
{
	int	i;

	i = 1;
	while (i < argc)
		if (is_invalid_number(argv[i++]))
			return (1);
	data->philo_number = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		data->number_forse_eat = ft_atoi(argv[5]);
	else
		data->number_forse_eat = ~(1 << 31);
	return (0);
}

void	destroy_data(t_data *data)
{
	int	i;

	free_philos_data(data->philos_datas, data->philo_number);
	free(data->philos);
	pthread_mutex_destroy(&data->alive_mutex);
	pthread_mutex_destroy(&data->print_mutex);
	i = 0;
	while (i < data->philo_number)
	{
		pthread_mutex_destroy(data->forks + i);
		i++;
	}
	free(data->forks);
}

int	init_data(t_data *data)
{
	static int	i = 0;

	data->philos = (pthread_t *)malloc(sizeof (pthread_t) * data->philo_number);
	if (!data->philos)
		return (1);
	data->forks = malloc(sizeof (pthread_mutex_t) * data->philo_number);
	if (!data->forks)
		return (free(data->philos), 1);
	memset(data->forks, 0, sizeof (*data->forks) * data->philo_number);
	if (pthread_mutex_init(&data->alive_mutex, NULL))
		return (free(data->philos), free(data->forks), 1);
	if (pthread_mutex_init(&data->print_mutex, NULL))
	{
		pthread_mutex_destroy(&data->alive_mutex);
		return (free(data->philos), free(data->forks), 1);
	}
	while (i < data->philo_number)
	{
		if (pthread_mutex_init(data->forks, NULL))
			return (destroy_data(data), 1);
		i++;
	}
	data->is_alive = 1;
	return (0);
}
