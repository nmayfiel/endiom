
// (C) Copyright 2018 by PSDRNDM, Inc. All Rights Reserved.
// Created by: Nickolas Mayfield @ PSDRNDM

/*
///////////////////////////////////////////////

  This is a C/C++ library
  
  To use this library:
     #define NDM_STRING_IMPLEMENTATION
     #include "ndm_string.h"
  in exactly one file where you want the C++ implementation code to be included.
  
  If necessary, you can include just the interface code by calling only
     #include "ndm_string.h"
  in as many files as you wish.

///////////////////////////////////////////////


*/


#ifndef NDM_STRING_H
# define NDM_STRING_H

#include <string.h>
#include <stdlib.h>
//#include "ndm_memory.h"

#define NDMSTR_ABS(X) ((X) < 0 ? -(X) : X)

#ifdef __cplusplus
extern "C" {
#endif

#define	ndms_isspace(c) ((c >= 011 && c <= 015) || c == 040)
#define ndms_isupper(c) (c >= 0101 && c <= 0132)
#define ndms_islower(c) (c >= 0141 && c <= 0172)
#define ndms_isalpha(c) (ndms_isupper(c) || ndms_islower(c))
#define ndms_isdigit(c) (c >= 060 && c <= 071)
#define ndms_isalnum(c) (ndms_isalpha(c) || ndms_isdigit(c))
#define ndms_isascii(c) (c >= 0 && c <= 0177)
#define	ndms_isprint(c) (c >= 040 && c <= 0176);

#define ndms_toupper(c) (ndms_isupper(c)) ? (c) : (c - 040)
#define ndms_tolower(c) (ndms_islower(c)) ? (c) : (c + 040)
#define ndms_charequ_ci(c1, c2) ((ndms_tolower(c1)) == (ndms_tolower(c2)))

    
size_t				ndms_strlen(const char *s);
size_t				ndms_numlen(int n);
size_t				ndms_numlen_base(int n, int base);
char				*ndms_strdup(const char *s1);
char				*ndms_strndup(const char *s1, size_t n);
char				*ndms_strcpy(char *dst, const char *src);
char				*ndms_strncpy(char *dst, const char *src, size_t len);
size_t				ndms_strlcpy(char *dst, const char *src, size_t size);
char				*ndms_strcat(char *s1, const char *s2);
char				*ndms_strncat(char *s1, const char *s2, size_t n);
size_t				ndms_strlcat(char *dst, const char *src, size_t size);
char				*ndms_strchr(const char *s, int c);
char				*ndms_strrchr(const char *s, int c);
char				*ndms_strstr(const char *big, const char *little);
char				*ndms_strnstr(const char *big, const char *little, size_t len);
char				*ndms_strcasestr(const char *big, const char *little);
int					ndms_strcmp(const char *s1, const char *s2);
int					ndms_strncmp(const char *s1, const char *s2, size_t n);
char				*ndms_strnew(size_t size);
void				ndms_strdel(char **as);
void				ndms_strclr(char *s);
void				ndms_striter(char *s, void (*f)(char *));
void				ndms_striteri(char *s, void (*f)(unsigned int, char *));
char				*ndms_strmap(char const *s, char (*f)(char));
char				*ndms_strmapi(char const *s, char (*f)(unsigned int, char));
int					ndms_strequ(char const *s1, char const *s2);
int					ndms_strnequ(char const *s1, char const *s2, size_t n);
char				*ndms_strsub(char const *s, unsigned int start, size_t len);
char				*ndms_strjoin(char const *s1, char const *s2);
char				*ndms_strtrim(char const *s);
char				**ndms_strsplit(char const *s, char c);
int					ndms_atoi(const char *str);
char				*ndms_itoa(int n);
char				*ndms_itoa_base(int value, int base);

void				ndms_eat_whitespace(char **str);
void				ndms_get_next_word(char **str, char *word);
    
#ifdef __cplusplus
}
#endif
    
#endif // NDM_STRING_H

#ifdef NDM_STRING_IMPLEMENTATION
#undef NDM_STRING_IMPLEMENTATION

#ifdef __cplusplus
#define ndms_str_inline inline
#else
#define ndms_str_inline
#endif

ndms_str_inline size_t		ndms_strlen(const char *s)
{
	int i;

	i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}

ndms_str_inline size_t		ndms_numlen(int n)
{
	size_t		c;

	if (n == 0)
		return (1);
	c = 0;
	if (n < 0)
		c++;
	while (n != 0)
	{
		n /= 10;
		++c;
	}
	return (c);
}

ndms_str_inline size_t	ndms_numlen_base(int value, int base)
{
	size_t			len;

	len = 0;
	if ((value < 0 && base == 10) || value == 0)
		len++;
	while (value != 0)
	{
		len++;
		value = value / base;
	}
	return (len);
}

char	*ndms_strdup(const char *s1)
{
	char	*dest;
	int		i;

	i = 0;
	dest = (char *)malloc(sizeof(char) * (ndms_strlen(s1) + 1));
	if (dest)
		while (s1[i] != '\0')
		{
			dest[i] = s1[i];
			i++;
		}
	else
		return (NULL);
	dest[i] = '\0';
	return (dest);
}

char	*ndms_strndup(const char *s1, size_t n)
{
	char	*dest;
	int		i;

	i = 0;
	dest = (char *)malloc(sizeof(char) * (n + 1));
	if (dest)
		while (s1[i] != '\0' && (size_t)i < n)
		{
			dest[i] = s1[i];
			i++;
		}
	else
		return (NULL);
	dest[i] = '\0';
	return (dest);
}

char	*ndms_strcpy(char *dst, const char *src)
{
	int	i;

	i = 0;
	while (src[i] != '\0')
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (dst);
}

char	*ndms_strncpy(char *dst, const char *src, size_t len)
{
	int i;

	i = 0;
	while ((size_t)i < len && src[i] != '\0')
	{
		dst[i] = src[i];
		i++;
	}
	while ((size_t)i < len)
		dst[i++] = '\0';
	return (dst);
}

size_t	ndms_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	i;

	i = 0;
	while (src[i] != '\0' && i < size - 1)
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (ndms_strlen(src));
}

char	*ndms_strcat(char *s1, const char *s2)
{
	int		i;

	i = 0;
	while (s1[i] != '\0')
		i++;
	while (*s2 != '\0')
	{
		s1[i] = *s2;
		i++;
		s2++;
	}
	s1[i] = '\0';
	return (s1);
}

char	*ndms_strncat(char *s1, const char *s2, size_t n)
{
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (s1[i] != '\0')
		i++;
	while (*s2 != '\0' && (size_t)j < n)
	{
		s1[i] = *s2;
		i++;
		j++;
		s2++;
	}
	s1[i] = '\0';
	return (s1);
}


size_t	ndms_strlcat(char *dst, const char *src, size_t size)
{
	int		i;
	int		j;
	int		len;

	i = 0;
	len = ndms_strlen(dst);
	i = len;
	j = 0;
	while (src[j] != '\0' && (size_t)i < size - 1)
	{
		dst[i] = src[j];
		i++;
		j++;
	}
	if (size == 0)
	{
		return (len);
		dst[0] = '\0';
	}
	dst[i] = '\0';
	if ((size_t)i > size - 1)
		return (size + ndms_strlen(src));
	return (len + ndms_strlen(src));
}

char	*ndms_strchr(const char *s, int c)
{
	while (*s != c && *s != '\0')
		s++;
	if (*s == c)
		return ((char *)s);
	else
		return (NULL);
}

char	*ndms_strrchr(const char *s, int c)
{
	int		i;

	i = 0;
	while (s[i] != '\0')
	{
		if (s[i] == c)
		{
			s += i;
			i = 0;
		}
		i++;
	}
	if (c == '\0')
		s += i;
	if (*s == c)
		return ((char *)s);
	else
		return (NULL);
}

char	*ndms_strstr(const char *big, const char *little)
{
	int		i;

	if (*little == '\0')
		return ((char *)big);
	while (*big != '\0')
	{
		i = 0;
		while (big[i] == little[i] && big[i] != '\0')
			i++;
		if (little[i] == '\0')
			return ((char *)big);
		big++;
	}
	return (NULL);
}

char	*ndms_strnstr(const char *big, const char *little, size_t len)
{
	int	i;
	int	j;

	if (*little == '\0')
		return ((char *)big);
	j = 0;
	while (*big != '\0' && (size_t)j < len)
	{
		i = 0;
		while (big[i] == little[i] && big[i] != '\0' && (size_t)(j + i) < len)
			i++;
		if (little[i] == '\0')
			return ((char *)big);
		j++;
		big++;
	}
	return (NULL);
}


char	*ndms_strcasestr(const char *big, const char *little)
{
	int	i;

	if (*little == '\0')
		return ((char *)big);
	while (*big != '\0')
	{
		i = 0;
		while (ndms_tolower(big[i]) == ndms_tolower(little[i]) && big[i] != '\0')
			i++;
		if (little[i] == '\0')
			return ((char *)big);
		big++;
	}
	return (NULL);
}


int		ndms_strcmp(const char *s1, const char *s2)
{
	while (*s1 != '\0' && *s2 != '\0')
	{
		if (*(unsigned char *)s1 != *(unsigned char *)s2)
			return (*(unsigned char *)s1 - *(unsigned char *)s2);
		s1++;
		s2++;
	}
	return (*(unsigned char *)s1 - *(unsigned char *)s2);
}


int		ndms_strncmp(const char *s1, const char *s2, size_t n)
{
	if (n == 0)
		return (0);
	while (*s1 != '\0' && *s2 != '\0' && n-- > 1)
	{
		if (*(unsigned char *)s1 != *(unsigned char *)s2)
			return (*(unsigned char *)s1 - *(unsigned char *)s2);
		s1++;
		s2++;
	}
	return (*(unsigned char *)s1 - *(unsigned char *)s2);
}


char	*ndms_strnew(size_t size)
{
	char *str;

	str = (char *)malloc(size + 1);
	if (!str)
		return (NULL);
	else
		memset(str, '\0', size + 1);
	return (str);
}


void	ndms_strdel(char **as)
{
	if (!as)
		return ;
    free(*as);
    *as = NULL;
}


void	ndms_strclr(char *s)
{
	if (!s)
		return ;
	while (*s != '\0')
	{
		*s = '\0';
		s++;
	}
}

void	ndms_striter(char *s, void (*f)(char *))
{
	if (!s || !f)
		return ;
	while (*s != '\0')
		(*f)(s++);
}

void	ndms_striteri(char *s, void (*f)(unsigned int, char *))
{
	int		i;

	if (!s || !f)
		return ;
	i = 0;
	while (s[i] != '\0')
	{
		(*f)((unsigned int)i, &s[i]);
		i++;
	}
}


char	*ndms_strmap(char const *s, char (*f)(char))
{
	char	*ret;
	int		i;

	if (!s || !f)
		return (NULL);
	ret = ndms_strnew(ndms_strlen(s));
	if (!ret)
		return (NULL);
	i = 0;
	while (s[i] != '\0')
	{
		ret[i] = (*f)(s[i]);
		i++;
	}
	ret[i] = '\0';
	return (ret);
}


char	*ndms_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char	*ret;
	int		i;

	if (!s || !f)
		return (NULL);
	ret = ndms_strnew(ndms_strlen(s));
	if (!ret)
		return (NULL);
	i = 0;
	while (s[i] != '\0')
	{
		ret[i] = (*f)((unsigned int)i, s[i]);
		i++;
	}
	return (ret);
}


int		ndms_strequ(char const *s1, char const *s2)
{
	if (!s1 || !s2)
		return (0);
	return (!ndms_strcmp(s1, s2));
}

int		ndms_strnequ(char const *s1, char const *s2, size_t n)
{
	if (!s1 || !s2)
		return (0);
	return (!strncmp(s1, s2, n));
}


char	*ndms_strsub(char const *s, unsigned int start, size_t len)
{
	char	*str;
	int		i;

	if (!s)
		return (NULL);
	str = (char *)malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	while (start--)
		s++;
	i = 0;
	while ((size_t)i < len)
	{
		str[i] = s[i];
		i++;
	}
	str[i] = '\0';
	return (str);
}


char	*ndms_strjoin(char const *s1, char const *s2)
{
	char	*join;
	int		i;
	int		j;

	if (!s1 || !s2)
		return (NULL);
	i = 0;
	j = 0;
	join = ndms_strnew(ndms_strlen(s1) + ndms_strlen(s2) + 1);
	if (!join)
		return (NULL);
	while (*(s1 + i) != 0)
		*(join + j++) = *(s1 + i++);
	i = 0;
	while (*(s2 + i) != 0)
		*(join + j++) = *(s2 + i++);
	*(join + j) = '\0';
	return (join);
}


char	*ndms_strtrim(char const *s)
{
	char		*trim;
	size_t		len;
	int			i;

	if (!s)
		return (NULL);
	while (*s == 011 || *s == 012 || *s == 040)
		s++;
	len = ndms_strlen(s);
	while (len > 0 &&
			(s[len - 1] == 011 || s[len - 1] == 012 || s[len - 1] == 040))
		len--;
	trim = ndms_strnew(len);
	if (!trim)
		return (NULL);
	i = 0;
	while ((size_t)i < len)
	{
		trim[i] = s[i];
		i++;
	}
	trim[i] = '\0';
	return (trim);
}


ndms_str_inline int		ndms__num_words(const char *s, char c)
{
	int		i;

	i = 0;
	if (!s)
		return (0);
	while (*s != '\0')
	{
		if (*s != c)
			++i;
		while (*s != c && *(s + 1) != '\0')
			++s;
		++s;
	}
	return (i);
}

char			**ndms_strsplit(const char *s, char c)
{
	char	**ret;
	int		len;
	int		i;
	char	c2;

	if (!s)
		return (NULL);
	c2 = c;
	while (*s == c && *s != '\0')
		++s;
	len = ndms__num_words(s, c);
	ret = (char **)malloc(sizeof(char *) * (len + 1));
	if (!ret)
		return (NULL);
	i = -1;
	while (++i < len)
	{
		c2 = ndms_strchr(s, c) == NULL ? '\0' : c;
		ret[i] = ndms_strndup(s, ndms_strchr(s, c2) - s);
		s = ndms_strchr(s, c2);
		while (*s == c && *(s + 1) != '\0')
			++s;
	}
	ret[i] = NULL;
	return (ret);
}


int		ndms_atoi(const char *str)
{
	long int		result;
	int				factor;

	factor = 1;
	result = 0;
	while (ndms_isspace(*str))
		str++;
	factor = *str == '-' && ndms_isdigit(str[1]) ? -factor : factor;
	str = (*str == '-' || *str == '+') && ndms_isdigit(str[1]) ? str + 1 : str;
	while (ndms_isdigit(*str))
	{
		result *= 10;
		result += *str - 48;
		str++;
	}
	return ((int)(result * factor));
}

char	*ndms_itoa(int n)
{
	char		*str;
	size_t		num_chars;

	num_chars = ndms_numlen(n);
	str = (char *)malloc(sizeof(char) * (num_chars + 1));
	if (!str)
		return (NULL);
	if (n < 0)
		str[0] = '-';
	if (n == 0)
		str[0] = '0';
	str[num_chars] = '\0';
	num_chars--;
	while (n != 0)
	{
		str[num_chars] = NDMSTR_ABS(n % 10) + '0';
		n /= 10;
		num_chars--;
	}
	return (str);
}


char		*ndms_itoa_base(int value, int base)
{
	char   	*ret;
	size_t 	len;
	size_t 	stop;
	char   	basestr[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	if (base < 2 || base > 36)
		return (NULL);
	stop = 0;
	len = ndms_numlen_base(value, base);
	ret = (char *)malloc(sizeof(char) * (len + 1));
	if (value < 0 && base == 10)
	{
		ret[0] = '-';
		++stop;
	}
	ret[len] = '\0';
	--len;
	while (len >= stop)
	{
		ret[len] = basestr[NDMSTR_ABS(value % base)];
		value /= base;
		--len;
	}
	return (ret);
}

void ndms_eat_whitespace(char **str)
{
    while(is_whitespace(**str))
        ++(*str);
}

void ndms_get_next_word(char **str, char *word)
{
    int i = 0;
    while (!is_whitespace(**str) && **str != '\0')
    {
        word[i] = **str;
        ++(*str);
        ++i;
    }
    word[i] = '\0';
}

#endif // NDM_STRING_IMPLEMENATION

