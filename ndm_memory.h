
// (C) Copyright 2018 by PSDRNDM, Inc. All Rights Reserved.
// Created by: Nickolas Mayfield @ PSDRNDM

/*
///////////////////////////////////////////////

  This is a C/C++ library
  
  To use this library:
     #define NDM_MEMORY_IMPLEMENTATION
     #include "ndm_memory.h"
  in exactly one file where you want the C++ implementation code to be included.
  
  If necessary, you can include just the interface code by calling only
     #include "ndm_memory.h"
  in as many files as you wish.

///////////////////////////////////////////////


*/

#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>

#ifndef NDM_MEMORY_H
# define NDM_MEMORY_H

#ifdef __cplusplus
extern "C" {
#endif

void				*ndmm_memset(void *b, int c, size_t len);
void				ndmm_bzero(void *s, size_t n);
void				*ndmm_memcpy(void *dst, const void *src, size_t n);
void				*ndmm_memccpy(void *dst, const void *src, int c, size_t n);
void				*ndmm_memcpyr(void *dst, const void *src, size_t n);
void				*ndmm_memmove(void *dst, const void *src, size_t len);
void				*ndmm_memchr(const void *s, int c, size_t n);
int					ndmm_memcmp(const void *s1, const void *s2, size_t n);
void				*ndmm_alloc(size_t size);
void				*ndmm_realloc(void *ptr, size_t size, size_t new_size);
void				ndmm_delete(void **ap);
void				ndmm_print_memory(const void *addr, size_t size);
void				ndmm_free_multi(int num, ...);
void				ndmm_swap(int *a, int *b);

#ifdef __cplusplus
}
#endif
    
#endif // NDM_MEMORY_H

#ifdef NDM_MEMORY_IMPLEMENTATION
#undef NDM_MEMORY_IMPLEMENTATION

#ifdef __cplusplus
#define ndmm_mem_inline inline
#else
#define ndmm_mem_inline
#endif

void	*ndmm_memset(void *b, int c, size_t len)
{
	unsigned char *mem;

	mem = (unsigned char *)b;
	while (len--)
	{
		*(unsigned char *)mem = (unsigned char)c;
		mem++;
	}
	return (b);
}

void	ndmm_bzero(void *s, size_t n)
{
	ndmm_memset(s, 0, n);
}

void	*ndmm_memcpy(void *dst, const void *src, size_t n)
{
    unsigned char	*uc_src;
    unsigned char	*uc_dst;

    uc_src = (unsigned char *)src;
    uc_dst = (unsigned char *)dst;
	while (n--)
		*(uc_dst++) = *(uc_src++);
	return (dst);
}

void	*ndmm_memccpy(void *dst, const void *src, int c, size_t n)
{
	unsigned char	*cdst;
	unsigned char	*csrc;

	cdst = (unsigned char *)dst;
	csrc = (unsigned char *)src;
	while (n > 0)
	{
		n--;
		*cdst = *csrc;
		if (*csrc == (unsigned char)c)
			return (++cdst);
		csrc++;
		cdst++;
	}
	return (NULL);
}

void	*ndmm_memcpyr(void *dst, const void *src, size_t n)
{
    unsigned char	*uc_dst;
    unsigned char	*uc_src;
	int				i;

    uc_src = (unsigned char *)src + n;
    uc_dst = (unsigned char *)dst + n;

	i = n;
	while (i > 0)
	{
		*uc_dst = *uc_src;
		uc_dst--;
		uc_src--;
		i--;
	}
	return (dst);
}

void		*ndmm_memmove(void *dst, const void *src, size_t len)
{
	unsigned char	*srcp;
	unsigned char	*dstp;
	size_t			diff;

	srcp = (unsigned char *)src;
	dstp = (unsigned char *)dst;
	diff = dstp - srcp;
	if (diff >= len)
		dst = ndmm_memcpy(dstp, srcp, len);
	else
		dst = ndmm_memcpyr(dstp, srcp, len);
	return (dst);
}

void	*ndmm_memchr(const void *s, int c, size_t n)
{
	unsigned char		*us;
	unsigned char		cc;

	us = (unsigned char *)s;
	cc = (unsigned char)c;
	while (n-- > 0)
	{
		if (*us == cc)
			return (us);
		us++;
	}
	return (NULL);
}

int		ndmm_memcmp(const void *s1, const void *s2, size_t n)
{
    unsigned char *uc_s1 = (unsigned char *)s1;
    unsigned char *uc_s2 = (unsigned char *)s2;

    if (n <= 0)
        return (0);
    while (n-- > 1)
    {
        if (uc_s1 != uc_s2)
            return (uc_s1 - uc_s2);
        ++uc_s1;
        ++uc_s2;
    }
    return (*uc_s1 - *uc_s2);
}

void	*ndmm_memalloc(size_t size)
{
	void	*ret;

	ret = malloc(size);
	if (ret)
		ndmm_bzero(ret, size);
	return (ret);
}

void		*ndmm_realloc(void *ptr, size_t size, size_t new_size)
{
	unsigned char	*ret;

	ret = (unsigned char *)malloc(sizeof(unsigned char) * new_size);
	if (!ret)
		return (NULL);
	if (size <= new_size)
		ndmm_memcpy(ret, ptr, size);
	else if (size > new_size)
		ndmm_memcpy(ret, ptr, new_size);
	free(ptr);
	return (ret);
}

void	ndmm_memdel(void **ap)
{
	free(*ap);
	*ap = NULL;
}

ndmm_mem_inline void		ndmm__print_charvals(unsigned char *arr, int start, size_t size)
{
	int		i;

	i = start;
	while (i < start + 16 && (size_t)i < size)
	{
		if (arr[i] > ' ' && arr[i] <= '~')
			write(1, &arr[i], 1);
		else
			write(1, ".", 1);
		++i;
	}
	write(1, "\n", 1);
}

ndmm_mem_inline void		ndmm__print_memline(unsigned char *arr, int i, size_t size)
{
    char base[] = "0123456789abcdef";

	write(1, &base[(arr[i] / 16)], 1);
	write(1, &base[(arr[i] % 16)], 1);
	if ((i + 1) % 2 == 0)
		write(1, " ", 1);
	if ((i + 1) % 16 == 0)
		ndmm__print_charvals(arr, i - 15, size);
}

ndmm_mem_inline void		ndmm__fill_spaces(unsigned char *arr, int i, size_t size)
{
	while ((i + 1) % 16 != 0)
	{
		if (i % 2 == 0)
			write(1, " ", 1);
		write(1, "  ", 2);
		++i;
	}
	ndmm__print_charvals(arr, i - 15, size);
}

void			ndmm_print_memory(const void *addr, size_t size)
{
	int				i;
	unsigned char	*arr;

	i = 0;
	arr = (unsigned char *)addr;
	while ((size_t)i < size)
	{
		ndmm__print_memline(arr, i, size);
		++i;
	}
	if (i % 16 != 0)
		ndmm__fill_spaces(arr, i - 1, size);
}


void	ndmm_free_multi(int num, ...)
{
	va_list	valist;
	int		i;

	va_start(valist, num);
	i = 0;
	while (i < num)
	{
		free(va_arg(valist, void *));
		++i;
	}
	va_end(valist);
}

void		ndmm_swap(int *a, int *b)
{
	int		tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

#endif // NDM_MEMORY_IMPLEMENATION

