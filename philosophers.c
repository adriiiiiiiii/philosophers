/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrgonza <adrgonza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 14:53:24 by adrgonza          #+#    #+#             */
/*   Updated: 2023/04/13 19:44:10 by adrgonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_get_values(t_data *data, int ac, char **av, int i)
{
	if (pthread_mutex_init(&(data->forks[i]), NULL))
		return (0);
	if (pthread_mutex_init(&(data->defender[i]), NULL))
		return (0);
	if (pthread_mutex_init(&(data->print_lock[i]), NULL))
		return (0);
	data->philo[i].time_to_die = ft_atoi_s(av[2]);
	data->philo[i].time_to_eat = ft_atoi_s(av[3]);
	data->philo[i].time_sleep = ft_atoi_s(av[4]);
	data->philo[i].star_time = data->start_time;
	data->philo[i].id = i + 1;
	data->philo[i].stop = &(data->stop);
	data->philo[i].right_fork = &(data->forks[i]);
	data->philo[i].left_fork = &(data->forks[(i + 1) % (data->philos_nb)]);
	data->philo[i].defender = &(data->defender[i]);
	data->philo[i].print_lock = data->print_lock;
	data->philo[i].alredy_eaten = 0;
	data->philo[i].times_eaten = 0;
	data->philo[i].nb_philos_eat = -1;
	if (ac == 6)
		data->philo[i].nb_philos_eat = ft_atoi_s(av[5]);
	return (1);
}

int	ft_create_threads(t_data *d, int ac, char **av)
{
	int	i;

	i = -1;
	pthread_mutex_lock(d->print_lock);
	while (++i < d->philos_nb)
	{
		if (!ft_get_values(d, ac, av, i))
			return (0);
		pthread_create(&(d->philosophers[i]), NULL, routine, &(d->philo[i]));
	}
	pthread_mutex_unlock(d->print_lock);
	return (1);
}

int	ft_init_data(t_data *d, char **av)
{
	d->philos_nb = ft_atoi_s(av[1]);
	d->philo = malloc(sizeof(t_philo) * (d->philos_nb));
	if (!d->philo)
		return (0);
	d->philosophers = malloc(sizeof(pthread_t) * (d->philos_nb));
	if (!d->philosophers)
		return (free(d->philo), 0);
	d->forks = malloc(sizeof(pthread_mutex_t) * (d->philos_nb));
	if (!d->forks)
		return (free(d->philo), free(d->philosophers), 0);
	d->defender = malloc(sizeof(pthread_mutex_t) * (d->philos_nb));
	if (!d->defender)
		return (free(d->philo), free(d->philosophers), free(d->forks), 0);
	d->print_lock = malloc(sizeof(pthread_mutex_t) * 1);
	d->stop = 1;
	if (!d->print_lock)
	{
		free(d->defender);
		return (free(d->philo), free(d->philosophers), free(d->forks), 0);
	}
	d->start_time = get_time();
	return (1);
}

int	ft_check_args(int argc, char **argv)
{
	int	i;
	int	j;

	if (argc < 5 || argc > 6)
		return (0);
		j = 0;
	while (++j < argc)
	{
		i = -1;
		while (argv[j][++i])
			if (argv[j][i] < '0' || argv[j][i] > '9')
				return (0);
	}
	if (!ft_atoi_s(argv[2]) || !ft_atoi_s(argv[3]) || !ft_atoi_s(argv[4]))
		return (0);
	if (argc == 6)
		if (!ft_atoi_s(argv[5]))
			return (0);
	return (1);
}

int	main(int argc, char **argv)
{
	t_data	data;

	if (!ft_check_args(argc, argv))
		return (printf("Error.\n"));
	if (!ft_init_data(&data, argv))
		return (printf("Malloc KO.\n"));
	if (!ft_create_threads(&data, argc, argv))
		return (ft_free(&data), 0);
	ft_paradise(&data);
	ft_destroy_threads(&data);
	ft_free(&data);
}
