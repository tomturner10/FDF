/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tturner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/07 16:54:31 by tturner           #+#    #+#             */
/*   Updated: 2021/09/07 16:54:31 by tturner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

static char	**ft_free(char **rtn, int i)
{
	int	j;

	j = 0;
	while (rtn[j] != NULL && j < i)
	{
		free(rtn[j]);
		j++;
	}
	free(rtn);
	return (NULL);
}

static int	ft_countwords(char const *s, char c)
{
	int	count;
	int	i;

	count = 0;
	i = 1;
	if (s == NULL || s[0] == 0)
		return (0);
	while (s[i] != '\0')
	{
		if (s[i - 1] == c && s[i] != c)
			count++;
		i++;
	}
	if (s[0] != c)
		count++;
	return (count);
}

static int	ft_nextword(char const *s, char c, int i)
{
	int	count;

	count = 0;
	while (s[i] == c && s[i] != '\0')
		i++;
	while (s[i] != c && s[i] != '\0')
	{
		count++;
		i++;
	}
	return (count);
}

char	**ft_split(char const *s, char c)
{
	char	**rtn;
	int		i;
	int		j;
	int		k;

	i = 0;
	k = 0;
	rtn = (char **)malloc((ft_countwords(s, c) + 1) * sizeof(char *));
	if (s == NULL || rtn == NULL)
		return (NULL);
	while (i < ft_countwords(s, c))
	{
		j = 0;
		rtn[i] = (char *)malloc(ft_nextword(s, c, k) + 1);
		if (rtn == NULL)
			return (ft_free(rtn, i));
		while (s[k] != '\0' && s[k] == c)
			k++;
		while (s[k] != '\0' && s[k] != c)
			rtn[i][j++] = s[k++];
		rtn[i][j] = '\0';
		i++;
	}
	rtn[i] = NULL;
	return (rtn);
}
