// NAME: Jeffrey Chan
// EMAIL: jeffschan97@gmail.com
// ID: 004-611638

#include "SortedList.h"
#include <pthread.h>
#include <string.h>
#include<stdlib.h>

void SortedList_insert(SortedList_t *list, SortedListElement_t *element)
{
	if(list == NULL || element == NULL) { return; } // check for invalid arugments
	SortedListElement_t *curr = list->next;

	// iterate through list and find position to insert element
	while(curr != list) {
		if(strcmp(element->key, curr->key) <= 0) break;

		curr  = curr->next;
	}

	if(opt_yield & INSERT_YIELD) sched_yield();

	// do pointer reconnection between list elements
	element->next = curr;
	element->prev = curr->prev;
	curr->prev->next = element;
	curr->prev = element;
}

int SortedList_delete(SortedListElement_t *element)
{
	if(element == NULL) { return 1; } // invalid arguments

	// check if pointers are corrupted, then try to delete element
	if(element->next->prev == element->prev->next) {
		if(opt_yield & DELETE_YIELD) sched_yield();

		// pointer reconnectino to delete element
		element->prev->next = element->next;
		element->next->prev = element->prev;
		return 0;
	}

	return 1; // failure to delete element
}

SortedListElement_t *SortedList_lookup(SortedList_t *list, const char *key)
{
	if(list == NULL || key == NULL) { return NULL; } // invalid arguments
	SortedListElement_t *curr = list->next;

	// iterate through list and try to find element
	while(curr != list) {
		if(strcmp(curr->key, key) == 0) return curr;
		
		if(opt_yield & LOOKUP_YIELD) sched_yield();
		
		curr = curr->next;
	}

	return NULL; // couldn't find element
}

int SortedList_length(SortedList_t* list) {
	if(list == NULL) { return -1; } // invalid arugments
	SortedListElement_t *curr = list->next;

	int count = 0;
	while(curr != list)
	{
		count++;

		if(opt_yield & LOOKUP_YIELD) sched_yield();
		
		curr = curr->next;
	}
	
	return count;
}
