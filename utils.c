/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfrants <vfrants@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 19:36:58 by vfrants           #+#    #+#             */
/*   Updated: 2023/11/14 22:01:04 by vfrants          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	stop_it_please(t_data *data)
{
	pthread_mutex_lock(&data->alive_mutex);
	data->is_alive = 0;
	pthread_mutex_unlock(&data->alive_mutex);
}

int	is_alive(t_philo *philo)
{
	int	res;

	pthread_mutex_lock(&philo->data->alive_mutex);
	res = 1;
	if (philo->data->is_alive == 0)
		res = 0;
	else if ((philo->last_meal
			&& get_millis() - philo->last_meal > philo->time_to_die))
	{
		print_action(philo, DIE);
		res = 0;
		philo->data->is_alive = 0;
	}
	pthread_mutex_unlock(&philo->data->alive_mutex);
	return (res);
}

void	print_action(t_philo *philo, int action)
{
	static int	flag = 1;
	static int	finished = 0;
	t_millis	time;

	pthread_mutex_lock(philo->print_mutex);
	if (action == DONE)
		finished++;
	time = get_millis() - philo->begin;
	if (finished == philo->data->philo_number && flag)
	{
		printf("%lld %i is eating\n", time, philo->id + 1);
		flag = 0;
		stop_it_please(philo->data);
	}
	if (action == FORK && flag)
		printf("%lld %i has taken a fork\n", time, philo->id + 1);
	else if (action == EAT && flag)
		printf("%lld %i is eating\n", time, philo->id + 1);
	else if (action == SLEEP && flag)
		printf("%lld %i is sleeping\n", time, philo->id + 1);
	else if (action == THINK && flag)
		printf("%lld %i is thinking\n", time, philo->id + 1);
	else if (action == DIE && flag)
		(free(NULL), flag = 0, printf("%lld %i died\n", time, philo->id + 1));
	pthread_mutex_unlock(philo->print_mutex);
}

t_millis	get_millis(void)
{
	t_time	current;

	gettimeofday(&current, NULL);
	return ((current.tv_sec * 1000) + (current.tv_usec / 1000));
}

void	free_philos_data(t_philo **data, int size)
{
	int	i;

	i = 0;
	while (i < size)
	{
		free(data[i]);
		i++;
	}
	free(data);
}
