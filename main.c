/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfrants <vfrants@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/19 12:24:08 by vfrants           #+#    #+#             */
/*   Updated: 2023/11/14 21:34:36 by vfrants          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	collect_philos(t_data *data)
{
	int	i;

	i = 0;
	pthread_mutex_lock(&data->alive_mutex);
	data->is_alive = 0;
	pthread_mutex_unlock(&data->alive_mutex);
	while (i < data->philo_number && data->philos[i])
	{
		pthread_join(data->philos[i], NULL);
		i++;
	}
}

t_philo	*setup_philo(t_data *data, int id)
{
	t_philo	*new;

	new = (t_philo *)malloc(sizeof (*new));
	if (!new)
		return (NULL);
	new->id = id;
	new->time_to_die = data->time_to_die;
	new->time_to_eat = data->time_to_eat;
	new->time_to_sleep = data->time_to_sleep;
	new->number_forse_eat = data->number_forse_eat;
	new->philo_number = data->philo_number;
	new->data = data;
	new->left = data->forks + id;
	new->eaten = 0;
	if (id == new->philo_number - 1)
		new->right = data->forks;
	else
		new->right = data->forks + id + 1;
	new->alive_mutex = &data->alive_mutex;
	new->print_mutex = &data->print_mutex;
	new->last_meal = 0;
	return (new);
}

void	setup_time(t_philo **data, int size)
{
	int	i;

	i = 0;
	while (i < size)
	{
		data[i]->begin = get_millis();
		i++;
	}
}

int	start_simulation(t_data *data)
{
	t_philo			**p_data;
	static int		i = -1;

	memset(data->philos, 0, sizeof (*data->philos) * data->philo_number);
	p_data = (t_philo **)malloc(sizeof (t_philo *) * data->philo_number);
	if (!p_data)
		return (destroy_data(data), 1);
	while (++i < data->philo_number)
	{
		p_data[i] = setup_philo(data, i);
		if (!p_data[i])
			return (free_philos_data(p_data, i - 1), destroy_data(data), 1);
	}
	i = 0;
	setup_time(p_data, data->philo_number);
	while (i < data->philo_number)
	{
		if (pthread_create(&data->philos[i], NULL, &routine, p_data[i]))
		{
			free_philos_data(p_data, data->philo_number);
			return (collect_philos(data), destroy_data(data), 1);
		}
		i++;
	}
	return (data->philos_datas = p_data, 0);
}

int	main(int argc, char **argv)
{
	t_data	data;

	if (argc < 5 || argc > 6 || parse_args(argc, argv, &data))
		return (write(2, "Invalid input\n", 14), 1);
	if (data.philo_number == 1)
		return (printf("0 1 has taken a fork\n0 1 died\n"), 0);
	if (init_data(&data) || start_simulation(&data))
		return (printf("Error occured during system call\n"), 1);
	while (1)
	{
		usleep(500);
		pthread_mutex_lock(&data.alive_mutex);
		if (!data.is_alive)
			break ;
		pthread_mutex_unlock(&data.alive_mutex);
	}
	pthread_mutex_unlock(&data.alive_mutex);
	collect_philos(&data);
	destroy_data(&data);
	return (0);
}
