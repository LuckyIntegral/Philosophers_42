/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_life.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfrants <vfrants@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 19:44:36 by vfrants           #+#    #+#             */
/*   Updated: 2023/11/14 21:59:23 by vfrants          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	philo_sleep(t_millis millis)
{
	if (millis < 0)
		return ;
	usleep(millis * 1000);
}

void	go_eating(t_philo *this)
{
	if (this->id % 2 == 0)
	{
		pthread_mutex_lock(this->right);
		print_action(this, FORK);
		if (!is_alive(this))
			return ((void)pthread_mutex_unlock(this->right));
		pthread_mutex_lock(this->left);
	}
	else
	{
		pthread_mutex_lock(this->left);
		print_action(this, FORK);
		if (!is_alive(this))
			return ((void)pthread_mutex_unlock(this->left));
		pthread_mutex_lock(this->right);
	}
	print_action(this, FORK);
	this->last_meal = get_millis();
	print_action(this, EAT);
	philo_sleep(this->time_to_eat);
	pthread_mutex_unlock(this->right);
	pthread_mutex_unlock(this->left);
}

void	*routine(void *data)
{
	t_philo		*this;

	this = (t_philo *)data;
	if (this->id % 2)
		usleep(100);
	while (is_alive(this))
	{
		go_eating(this);
		if (++this->eaten == this->number_forse_eat)
		{
			print_action(this, DONE);
			break ;
		}
		if (!is_alive(this))
			break ;
		print_action(this, SLEEP);
		philo_sleep(this->time_to_sleep);
		if (!is_alive(this))
			break ;
		print_action(this, THINK);
		philo_sleep(1);
	}
	return (NULL);
}
