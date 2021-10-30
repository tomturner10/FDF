/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tturner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/20 13:45:48 by tturner           #+#    #+#             */
/*   Updated: 2021/10/20 13:45:48 by tturner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static unsigned int	ft_strlcpy(char *dest, char *src, unsigned int size)
{
	unsigned int	i;

	i = 0;
	while (src[i])
		i++;
	if (size == 0)
		return (i);
	i = 0;
	while (src[i] && i < size - 1)
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	i = 0;
	while (src[i])
		i++;
	return (i);
}

int	ft_strlen(char *s)
{
	int	i;

	i = 0;
	if (s == NULL)
		return (i);
	while (s[i] != '\0')
		i++;
	return (i);
}

int	ft_strchr(char *s, int c)
{
	int	i;

	i = 0;
	if (s == NULL)
		return (0);
	while (s[i] != '\0' )
	{
		if (s[i] == (char)c)
			return (1);
		i++;
	}
	if (s[i] == c)
		return (1);
	return (0);
}

char	*ft_strjoin(char *line, char *buf)
{
	char	*rtn;
	int		i;
	int		j;

	if (buf == NULL)
		return (NULL);
	i = 0;
	j = 0;
	rtn = (char *)malloc(sizeof(char) * (ft_strlen(line) + ft_strlen(buf) + 1));
	while (line[i] != '\0')
	{
		rtn[i] = line[i];
		i++;
	}
	while (buf[j] != '\0')
	{
		rtn[i + j] = buf[j];
		j++;
	}
	rtn[i + j] = '\0';
	free(line);
	return (rtn);
}

char	*ft_strdup(char *s1)
{
	char			*str;
	unsigned int	s1len;

	s1len = 0;
	while (s1[s1len] != '\0')
		s1len++;
	str = (char *)malloc((s1len + 1) * sizeof(char));
	if (str == NULL)
		return (NULL);
	ft_strlcpy(str, (char *)s1, s1len + 1);
	return (str);
}
