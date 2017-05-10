// NAME: Jeffrey Chan
// EMAIL: jeffschan97@gmail.com
// ID: 004-611-638

#include"SortedList.h"
#include<stdlib.h>
#include<string.h>
#include<pthread.h>

void SortedList_insert(SortedList_t *list, SortedListElement_t *element) {
    if (list == NULL || element == NULL) { return; } // invalid arguments
    SortedListElement_t *prev = list; // prev will point to list element just before curr
    SortedListElement_t *curr = list->next; // set curr to first list element

    while (curr != NULL) {
	// point curr to first element that has a key greater than element's key
	if (strcmp(element->key, curr->key) <= 0) break;

	// move curr nad prev to next node
	curr = curr->next;
	prev = prev->next;
    }

    if (opt_yield & INSERT_YIELD) sched_yield(); // yield if needed

    // pointer reconnection to insert element into list
    prev->next = element;
    element->prev = prev;
    element->next = curr;
    if (curr != NULL) curr->prev = element;
}

int SortedList_delete(SortedListElement_t *element) {
    if (element == NULL) { return 1; } // invalid arguments

    // check that pointers aren't corrupted
    if (element->prev->next == element) {
	// case: last element in list
	if (element->next == NULL) {
	    if (opt_yield & DELETE_YIELD) sched_yield(); // yield if necessary

	    element->prev->next = NULL; // detach last element in list
	    return 0;
	}
	// case: element inbetween to other valid elements
	else if (element->next->prev == element) {
	    if (opt_yield & DELETE_YIELD) sched_yield(); // yield if necessary

	    // detach element from list
	    element->prev->next = element->next;
	    element->next->prev = element->prev;
	    return 0;
	}
    }
    
    return 1; // corrupted pointers
}

SortedListElement_t* SortedList_lookup(SortedList_t* list, const char *key) {
    if (list == NULL || key == NULL) { return NULL; } // invalid argument
    SortedListElement_t* curr = list->next; // declare list iterator

    while (curr != NULL ) {
	if (strcmp(curr->key, key) == 0) return curr; // found element

	if (opt_yield & LOOKUP_YIELD) sched_yield(); // yield if needed
	
	curr = curr->next; // move onto next element
    }
    
    return NULL; // coun't find element
}

int SortedList_length(SortedList_t *list) {
    if (list == NULL) return -1; // invalid arguments

    int counter = 0;
    SortedListElement_t* curr = list->next;

    while (curr != NULL) {
        counter++;
        // check pointers in case list is corrupted
        if (curr->prev->next != curr || (curr->next != NULL && curr->next->prev != curr))
            return -1;
        if (opt_yield & LOOKUP_YIELD) sched_yield();
        curr = curr->next;
    }
    
    return counter;
}
