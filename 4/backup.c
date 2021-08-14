#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "ics.h"
#include "emalloc.h"
#include "listy.h"

void print_event(node_t *n, void *arg) {
    assert(n != NULL);

    event_t *event = n->val;

    if (strcmp(event->rrule, "") == 0) {
        printf("EVENT: %s %s '%.10s' '%.10s'\n", event->dtstart,
            event->dtend, event->summary, event->location);
    } else {
        printf("EVENT: %s %s '%.10s' '%.10s' '%.10s'\n", event->dtstart,
            event->dtend, event->summary, event->location,
            event->rrule);
    }
}

int main(int argc, char *argv[])
{
    event_t *temp_event = NULL;
    node_t  *temp_node  = NULL;
    node_t  *head = NULL;

    /* Add one event, without an RRULE */

    temp_event = emalloc(sizeof(event_t));
    strncpy(temp_event->dtstart, "20190706T120000", 17);
    strncpy(temp_event->dtend, "20190706T160000", 17);
    strncpy(temp_event->summary, "summary 1", SUMMARY_LEN);
    strncpy(temp_event->location, "location 1", LOCATION_LEN);
    temp_event->rrule[0] = '\0';
    temp_node = new_node(temp_event);
    head = add_front(head, temp_node);

    /* Add a second event, with an RRULE */

    temp_event = emalloc(sizeof(event_t));
    strncpy(temp_event->dtstart, "20190707T093000", 17);
    strncpy(temp_event->dtend, "20190707T102000", 17);
    strncpy(temp_event->summary, "uvwxyz 1234567", SUMMARY_LEN);
    strncpy(temp_event->location, "abcde 1234567", LOCATION_LEN);
    strncpy(temp_event->rrule, "yada RULE yada UNTIL yada", RRULE_LEN);
    temp_node = new_node(temp_event);
    head = add_front(head, temp_node);
	
    temp_event = emalloc(sizeof(event_t));
    strncpy(temp_event->dtstart, "20190707T093000", 17);
    strncpy(temp_event->dtend, "20190707T102000", 17);
    strncpy(temp_event->summary, "uvwxyz 1234567", SUMMARY_LEN);
    strncpy(temp_event->location, "abcde 1234567", LOCATION_LEN);
    strncpy(temp_event->rrule, "I fucking handle memory allocation in C", RRULE_LEN);
    temp_node = new_node(temp_event);
    head = add_front(head, temp_node);
	
	apply(head, print_event, NULL);

    /* Free up the memory. This is done rather deliberately
     * and manually. A full-featured function might better
     * serve for this. Asserts are liberally used here as they
     * express state that *must* be true if all of the code is
     * correctly working.
     */
    temp_node = head;
    assert(temp_node != NULL);
    head = remove_front(head);
    temp_event = temp_node->val;
    assert(temp_event != NULL);
    free(temp_event);
    free(temp_node);

    temp_node = head;
    assert(temp_node != NULL);
    head = remove_front(head);
    temp_event = temp_node->val;
    assert(temp_event != NULL);
    free(temp_event);
    free(temp_node);
 
    temp_node = head;
    assert(temp_node != NULL);
    head = remove_front(head);
    temp_event = temp_node->val;
    assert(temp_event != NULL);
    free(temp_event);
    free(temp_node);
 
    assert(head == NULL); 	
	
}