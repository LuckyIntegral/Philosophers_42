/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfrants <vfrants@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/19 12:04:29 by vfrants           #+#    #+#             */
/*   Updated: 2023/11/14 21:02:15 by vfrants          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

// for: memset
# include <string.h>

// for: printf
# include <stdio.h>

// for: malloc, free
# include <stdlib.h>

// for: write, usleep
# include <unistd.h>

// for: gettimeeofday
# include <sys/time.h>

// for: pthread_create, pthread_detach, pthread_mutex_init,
//      pthread_mutex_destroy, pthread_mutex_lock, pthread_mutex_unlock
# include <pthread.h>

typedef struct timeval	t_time;
typedef long long		t_millis;

typedef enum s_actions
{
	FORK,
	EAT,
	SLEEP,
	THINK,
	DIE,
	DONE
}	t_action;

typedef struct s_data	t_data;

typedef struct s_philo
{
	int					id;
	int					philo_number;
	int					eaten;
	t_millis			time_to_die;
	t_millis			time_to_eat;
	t_millis			time_to_sleep;
	int					number_forse_eat;
	t_millis			last_meal;
	t_millis			begin;
	pthread_mutex_t		*left;
	pthread_mutex_t		*right;
	pthread_mutex_t		*alive_mutex;
	pthread_mutex_t		*print_mutex;
	t_data				*data;
}	t_philo;

typedef struct s_data
{
	t_philo				**philos_datas;
	pthread_t			*philos;
	pthread_mutex_t		*forks;
	t_millis			begin;
	pthread_mutex_t		alive_mutex;
	pthread_mutex_t		print_mutex;
	int					is_alive;
	int					number_forse_eat;
	int					time_to_sleep;
	int					philo_number;
	int					time_to_die;
	int					time_to_eat;
}	t_data;

// philosophers life cycle
void		*routine(void *data);

// parser and init of args
int			parse_args(int argc, char **argv, t_data *data);
void		destroy_data(t_data *data);
int			init_data(t_data *data);

// utils
void		free_philos_data(t_philo **data, int size);
void		print_action(t_philo *philo, int action);
int			is_alive(t_philo *philo);
t_millis	get_millis(void);

#endif
