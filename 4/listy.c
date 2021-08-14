/*
 * linkedlist.c
 *
 * Based on the implementation approach described in "The Practice 
 * of Programming" by Kernighan and Pike (Addison-Wesley, 1999).
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ics.h"
#include "emalloc.h"
#include "listy.h"


node_t *new_node(event_t *val) {
    assert( val != NULL);

    node_t *temp = (node_t *)emalloc(sizeof(node_t));

    temp->val = val;
    temp->next = NULL;

    return temp;
}

int cmp(node_t *a, node_t *b){
	if (strncmp(a->val->dtstart, b->val->dtstart, 15) == 0)
		return strncmp(a->val->summary, b->val->summary, 80);
	return strncmp(a->val->dtstart, b->val->dtstart, 15);
}

node_t *add_inorder(node_t *head, node_t *new){
	if(head == NULL)
		return new;

	new->next = head;	
	while(new->next != NULL && cmp(new, new->next) > 0){
		event_t *temp = new->val;
		new->val = new->next->val;
		new->next->val = temp;
		new = new->next;
	}

}



node_t *add_front(node_t *list, node_t *new) {
    new->next = list;
    return new;
}


node_t *add_end(node_t *list, node_t *new) {
    node_t *curr;

    if (list == NULL) {
        new->next = NULL;
        return new;
    }

    for (curr = list; curr->next != NULL; curr = curr->next);
    curr->next = new;
    new->next = NULL;
    return list;
}


node_t *peek_front(node_t *list) {
    return list;
}


node_t *remove_front(node_t *list) {
    if (list == NULL) {
        return NULL;
    }

    return list->next;
}



void apply(node_t *list,
           void (*fn)(node_t *list, void *),
           void *arg)
{
    for ( ; list != NULL; list = list->next) {
        (*fn)(list, arg);
    }
}
