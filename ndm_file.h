
// (C) Copyright 2018 by PSDRNDM, Inc. All Rights Reserved.
// Created by: Nickolas Mayfield @ PSDRNDM

/*
///////////////////////////////////////////////

This is a C/C++ library
  
To use this library:
#define NDM_FILE_IMPLEMENTATION
#include "ndm_file.h"
in exactly one file where you want the C++ implementation code to be included.
  
If necessary, you can include just the interface code by calling only
#include "ndm_file.h"
in as many files as you wish.

///////////////////////////////////////////////

@TODOS:

Pull out stdlib function, malloc and memcpy into macros so user can define their own.

*/


#ifndef NDM_FILE_H
# define NDM_FILE_H

#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

# define NDMF__BUFF_SIZE 1024

    typedef struct		ndmf__fh_s
    {
        char	**overs;
        int		*fds;
        int		num_fds;
    }					ndmf__fh;

    int					ndmf_get_next_line(const int fd, char **line);
    
#ifdef __cplusplus
}
#endif
    
#endif // NDM_FILE_H

#ifdef NDM_FILE_IMPLEMENTATION
#undef NDM_FILE_IMPLEMENTATION

#ifdef __cplusplus
#define ndmf_inline inline
#else
#define ndmf_inline
#endif


/*
** read_until_newline is a recursive helper that fills line string with
** the characters up to, but not including the first newline it finds
** it fills the overflow, or null if there isn't one
** finally, returns 1 on success, 0 on fail, -1 on error
*/

static int		ndmf__read_until_newline(const int fd, char **line, char **overflow)
{
	int		num_read;
	char	buf[NDMF__BUFF_SIZE + 1];
	char	*cat_str;

	if ((num_read = read(fd, buf, NDMF__BUFF_SIZE)) > 0)
	{
		buf[num_read] = '\0';
		if (strchr(buf, '\n') != NULL)
		{
			cat_str = strndup(buf, strchr(buf, '\n') - buf);
			*overflow = strdup(strchr(buf, '\n') + 1);
		}
		else
			cat_str = strdup(buf);
        if (*line)
        {
            *line = (char *)realloc(*line, sizeof(char) * (strlen(*line) + strlen(cat_str) + 1));
            *line = strncat(*line, cat_str, strlen(cat_str));
        }
        else
        {
            *line = strdup(cat_str);
        }
//		*line = ft_mallocat(*line, cat_str);
		free(cat_str);
		if (strchr(buf, '\n') == NULL && num_read == NDMF__BUFF_SIZE)
			return (ndmf__read_until_newline(fd, line, overflow));
		return (1);
	}
	if (num_read < 0)
		return (-1);
	if (*line)
		return (1);
    
	return (0);
}

/*
** get_fh checks if the file handle struct exists
** on the heap, if not, mallocs it, then fills it
** with default values
** if it does exist, checks whether the current fd
** has been opened before, if not saves it
** returns the index for the current fd
*/

static int		ndmf__get_fh(ndmf__fh *fhs, const int fd)
{
	int		i;

	if (fhs->num_fds == 0)
	{
		fhs->fds = (int *)malloc(sizeof(int));
		fhs->fds[0] = fd;
		fhs->overs = (char **)malloc(sizeof(char *));
		fhs->overs[0] = NULL;
		fhs->num_fds = 1;
		return (0);
	}
	i = -1;
	while (++i < fhs->num_fds)
		if (fhs->fds[i] == fd)
			return (i);
	fhs->fds = (int *)realloc(fhs->fds, sizeof(int) * (i + 1));
	fhs->overs = (char **)realloc(fhs->overs, sizeof(char *) * (i + 1));
	fhs->fds[i] = fd;
	fhs->overs[i] = NULL;
	fhs->num_fds++;
	return (i);
}

static char		*ndmf__set_line(char *lstr, char **overflow, char *ostr, char **tmp)
{
	char *line;

	line = lstr;
	free(*overflow);
	*overflow = ostr;
	free(*tmp);
	return (line);
}

/*
** Perform necessary checks and setup
** the initial variables before getting the next line
*/

static int		ndmf__setup_gnl(char **line, int *fi, ndmf__fh *fhs, const int fd)
{
	if (fd < 0 || line == NULL)
		return (-1);
	*line = NULL;
	*fi = ndmf__get_fh(fhs, fd);
	return (0);
}

/*
** get_next_line takes a file descriptor and a pointer to a string
** then fills string with the characters before the next newline
** in the file
*/

int				ndmf_get_next_line(const int fd, char **line)
{
	static ndmf__fh		fhs;
	int				fi;
	char			*tmp;

	if (ndmf__setup_gnl(line, &fi, &fhs, fd) != 0)
		return (-1);
	tmp = fhs.overs[fi] ? strdup(fhs.overs[fi]) : NULL;
    if (tmp && *tmp == '\n')
	{
		*line = ndmf__set_line(strdup("\0"), &fhs.overs[fi],
				strdup(tmp + 1), &tmp);
		return (1);
	}
	else if (tmp && strchr(tmp, '\n') != NULL)
	{
		*line = ndmf__set_line(strndup(tmp, strchr(tmp, '\n') - tmp),
				&fhs.overs[fi], strdup(strchr(tmp, '\n') + 1), &tmp);
		return (1);
	}
	else if (tmp && *tmp != '\0')
		*line = ndmf__set_line(strdup(fhs.overs[fi]), &fhs.overs[fi], NULL, &tmp);
	else if (tmp != NULL)
        free(tmp);

	return (ndmf__read_until_newline(fd, line, &fhs.overs[fi]));
}

#endif // NDM_FILE_IMPLEMENATION

