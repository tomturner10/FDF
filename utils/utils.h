/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tturner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/20 13:45:48 by tturner           #+#    #+#             */
/*   Updated: 2021/10/20 13:45:48 by tturner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>

char	*get_next_line(int fd, int buffsize);
char	*ft_strjoin(char *line, char *buf);
int		ft_strchr(char *str, int c);
char	*ft_strdup(char *str);
int		ft_strlen(char *s);
char	**ft_split(char const *s, char c);

#endif
