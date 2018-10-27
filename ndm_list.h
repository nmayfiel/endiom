
// (C) Copyright 2018 by PSDRNDM, Inc. All Rights Reserved.
// Created by: Nickolas Mayfield @ PSDRNDM

/*
///////////////////////////////////////////////

  This is a C/C++ library
  
  To use this library:
     #define NDM_LIST_IMPLEMENTATION
     #include "ndm_list.h"
  in exactly one file where you want the C++ implementation code to be included.
  
  If necessary, you can include just the interface code by calling only
     #include "ndm_list.h"
  in as many files as you wish.

///////////////////////////////////////////////

@TODOS:

Pull out stdlib function, malloc and memcpy into macros so user can define their own.

*/


#ifndef NDM_LIST_H
# define NDM_LIST_H

#include <string.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct		ndm_list_s
{
	void				*content;
	size_t				content_size;
	struct ndm_list_s	*next;
}					ndm_list_t;

ndm_list_t			*ndmlst_new(void const *content, size_t content_size);
void				ndmlst_delete_one_element(ndm_list_t **list_base, void (*del)(void *, size_t));
void				ndmlst_delete_entire_list(ndm_list_t **list_base, void (*del)(void *, size_t));
void				ndmlst_push_front(ndm_list_t **list_base, ndm_list_t *elem);
void				ndmlst_push_back(ndm_list_t **list_base, ndm_list_t *elem);
void				ndmlst_iterate(ndm_list_t *list_base, void (*f)(ndm_list_t *elem));
ndm_list_t			*ndmlst_map(ndm_list_t *list_base, ndm_list_t *(*f)(ndm_list_t *elem));

    
#ifdef __cplusplus
}
#endif
    
#endif // NDM_LIST_H

#ifdef NDM_LIST_IMPLEMENTATION
#undef NDM_LIST_IMPLEMENTATION

#ifdef __cplusplus
#define ndm_lst_inline inline
#else
#define ndm_lst_inline
#endif

ndm_list_t	*ndmlst_new(void const *content, size_t content_size)
{
	ndm_list_t		*list;

	list = (ndm_list_t *)malloc(sizeof(ndm_list_t));
	if (!list)
		return (NULL);
	if (content)
	{
		list->content = malloc(content_size);
		if (!list->content)
			return (NULL);
		memcpy(list->content, content, content_size);
		list->content_size = content_size;
	}
	else
	{
		list->content = NULL;
		list->content_size = 0;
	}
	list->next = NULL;
	return (list);
}

void	ndmlst_delete_one_element(ndm_list_t **elem, void (*del)(void *, size_t))
{
	(*del)((*elem)->content, (*elem)->content_size);
	free(*elem);
	*elem = NULL;
}

void	ndmlst_delete_entire_list(ndm_list_t **list_base, void (*del)(void *, size_t))
{
	if ((*list_base)->next)
		ndmlst_delete_entire_list(&(*list_base)->next, del);
	ndmlst_delete_one_element(list_base, del);
}

void	ndmlst_push_front(ndm_list_t **list_base, ndm_list_t *elem)
{
	elem->next = *list_base;
	*list_base = elem;
}

void	ndmlst_push_back(ndm_list_t **list_base, ndm_list_t *elem)
{
    ndm_list_t *the_list = *list_base;

    while(the_list->next != NULL)
        the_list = the_list->next;
    the_list->next = elem;
}

void	ndmlst_iterate(ndm_list_t *lst, void (*f)(ndm_list_t *elem))
{
	while (lst != NULL)
	{
		(*f)(lst);
		lst = lst->next;
	}
}

ndm_list_t	*ndmlst_map(ndm_list_t *lst, ndm_list_t *(*f)(ndm_list_t *elem))
{
	ndm_list_t	*new_list;
	ndm_list_t	*ret;

	new_list = (*f)(lst);
	if (!new_list)
		return (NULL);
	ret = new_list;
	lst = lst->next;
	while (lst != NULL)
	{
		new_list->next = (*f)(lst);
		new_list = new_list->next;
		if (!new_list)
			return (NULL);
		lst = lst->next;
	}
	return (ret);
}

#endif // NDM_LIST_IMPLEMENATION

