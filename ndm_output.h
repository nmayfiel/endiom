
// (C) Copyright 2018 by PSDRNDM, Inc. All Rights Reserved.
// Created by: Nickolas Mayfield @ PSDRNDM

/*
///////////////////////////////////////////////

  This is a C/C++ library
  
  To use this library:
     #define NDM_OUTPUT_IMPLEMENTATION
     #include "ndm_output.h"
  in exactly one file where you want the C++ implementation code to be included.
  
  If necessary, you can include just the interface code by calling only
     #include "ndm_output.h"
  in as many files as you wish.

///////////////////////////////////////////////


*/

#include <unistd.h>

#ifndef NDM_OUTPUT_H
# define NDM_OUTPUT_H

#ifdef __cplusplus
extern "C" {
#endif

void				ndmo_putchar_fd(char c, int fd);
void				ndmo_putstr_fd(char const *s, int fd);
void				ndmo_putendl_fd(char const *s, int fd);
void				ndmo_putnbr_fd(int n, int fd);

#define				ndmo_putchar(x)	ndmo_putchar_fd(x, 1)
#define				ndmo_putstr(x)	ndmo_putstr_fd(x, 1)
#define 			ndmo_putendl(x)	ndmo_putendl_fd(x, 1)
#define				ndmo_putnbr(x)	ndmo_putnbr_fd(x, 1)
    
#ifdef __cplusplus
}
#endif
    
#endif // NDM_OUTPUT_H

#ifdef NDM_OUTPUT_IMPLEMENTATION
#undef NDM_OUTPUT_IMPLEMENTATION

void	ndmo_putchar_fd(char c, int fd)
{
	write(fd, &c, sizeof(c));
}

void	ndmo_putstr_fd(char const *s, int fd)
{
	while (*s != '\0')
	{
		ndmo_putchar_fd(*s, fd);
		s++;
	}
}

void	ndmo_putendl_fd(char const *s, int fd)
{
	ndmo_putstr_fd(s, fd);
	ndmo_putchar_fd('\n', fd);
}

// @TODO Make this work with 64 bit values or more...
void	ndmo_putnbr_fd(int n, int fd)
{
	if (n == -2147483648)
	{
		ndmo_putchar_fd('-', fd);
		ndmo_putchar_fd('2', fd);
		n %= 1000000000;
		n = -n;
	}
	if (n < 0)
	{
		ndmo_putchar_fd('-', fd);
		n = -n;
	}
	if (n >= 10)
	{
		ndmo_putnbr_fd(n / 10, fd);
		ndmo_putnbr_fd(n % 10, fd);
	}
	else
		ndmo_putchar_fd(n + '0', fd);
}

#endif // NDM_OUTPUT_IMPLEMENATION

