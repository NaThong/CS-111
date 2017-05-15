/*
 * SortedList (and SortedListElement)
 *
 *	A doubly linked list, kept sorted by a specified key.
 *	This structure is used for a list head, and each element
 *	of the list begins with this structure.
 *
 *	The list head is in the list, and an empty list contains
 *	only a list head.  The list head is also recognizable because
 *	it has a NULL key pointer.
 */

struct SortedListElement {
	struct SortedListElement *prev;
	struct SortedListElement *next;
	const char *key;
};
typedef struct SortedListElement SortedList_t;
typedef struct SortedListElement SortedListElement_t;

/* METHOD DECLARATIONS */

void SortedList_insert(SortedList_t *list, SortedListElement_t *element);

int SortedList_delete( SortedListElement_t *element);

SortedListElement_t *SortedList_lookup(SortedList_t *list, const char *key);

int SortedList_length(SortedList_t *list);

// VARIABLE TO ENABLE DIAGNOSTIC YIELD CALLS

extern int opt_yield;
#define	INSERT_YIELD	0x01	// yield in insert critical section
#define	DELETE_YIELD	0x02	// yield in delete critical section
#define	LOOKUP_YIELD	0x04	// yield in lookup/length critical esction
