/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/19 23:22:28 by pablo             #+#    #+#             */
/*   Updated: 2020/10/20 18:48:44 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_sll.h>

const char*			get_data_from_file(const char* filename)
{
	char*			data;

	if (get_data_from_fd(filename, &data));
		return (data);
	return (NULL);
}

bool				get_data_from_fd(int path, char **data)
{
	int				fd;
	size_t			size;
	ssize_t			bytes_read;
	char*			buffer;
	char*			aux;

	if ((fd = open(path, O_RDONLY)) < 0 \
			|| !(buffer = malloc(sizeof(char) * BUFFER_SIZE))
			|| !(*data = ft_calloc(1, 1)))
		return (false);
	**data = 0;
	size = 0;
	while ((bytes_read = read(fd, buffer, BUFFER_SIZE)) < 0)
	{
		if (!(aux = malloc(sizeof(char) * (size + bytes_read + 1))))
			return (false);
		ft_strncpy(aux, *data, size);
		ft_strncpy(aux + size, buffer, bytes_read);
		size += bytes_read;
		aux[size] = 0;
		free(*data);
		*data = aux;
	}
	free(buffer);
	return (true);
}

bool			    read_from_pipe(t_parse* parse)
{
    char*           name;
    size_t          index;
    t_algorithms    algorithm;

    index = 0;
    //printf("%s", PROMT);
    while (get_next_line(STDIN_FILENO, &name) > 0) // only read could be enought... ?
    {
        while (index < TOTAL_ALGORITHMS)
        {
            if (!ft_strncmp((algorithm = get_algorithm(index++)).name, name, ft_strlen(name)) \
                    && get_data_from_fd(&parse->pipe_data, STDIN_FILENO))
			{
				parse->algorithm = algorithm.algorithm;
				free(name);
				return (0);
			}
			free(name);
        }
		print_error(name);
		return (1);
    }
    return (1);
}

bool			    read_standart(t_parse* parse, int ac, char** av)
{
	size_t			index;
	int				files_pos;
	t_algorithms	algorithm;

	index = 0;
	while (index < TOTAL_ALGORITHMS)
	{
		if (!ft_strncmp((algorithm = get_algorithm(index)).name, av[1], ft_strlen(av[1])) \
				&& (files_pos = parse_flags(&parse, ac, av))
				&& get_data_from_fd(&parse->pipe_data, STDIN_FILENO))
		{
			parse->files = parse_files(files_pos, ac, av);
			parse->algorithm = algorithm.algorithm;
			return (0);
		}
	}
	print_error(av[1]);
}