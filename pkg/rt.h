// Header guard to prevent multiple inclusion
#ifndef __RT__
#define __RT__

// Included header files for standard types and custom timer library
#include <stdint.h>
#include <stdbool.h>
#include "timerlib.h"

// Define expiration time for a routing table entry (in seconds)
#define RT_TABLE_EXP_TIME	30  /*  30 sec */

// Structure to represent keys of a routing table entry
typedef struct rt_entry_keys_{
    char dest[16];  // Destination IP address
    char mask;      // Subnet mask
} rt_entry_keys_t;

// Structure to represent a routing table entry
typedef struct rt_entry_{
    rt_entry_keys_t rt_entry_keys;  // Structure containing only keys (dest and mask)
    char gw_ip[16];                 // Gateway IP address
    char oif[32];                   // Output interface name
	uint32_t time_to_expire;         // Time left before entry is deleted
    struct rt_entry_ *prev;         // Pointer to previous entry
    struct rt_entry_ *next;         // Pointer to next entry
	Timer_t *exp_timer;              // Pointer to expiration timer
} rt_entry_t;

// Structure to represent a routing table
typedef struct rt_table_{
    rt_entry_t *head;  // Pointer to the first entry in the table
} rt_table_t;

// Function prototypes
void rt_init_rt_table(rt_table_t *rt_table);
bool rt_add_new_rt_entry(rt_table_t *rt_table,
    char *dest_ip, char mask, char *gw_ip, char *oif,
	void (*timer_cb)(Timer_t*, void *));
bool rt_delete_rt_entry(rt_table_t *rt_table,
    char *dest_ip, char mask);
bool rt_update_rt_entry(rt_table_t *rt_table,
    char *dest_ip, char mask, 
    char *new_gw_ip, char *new_oif);
void rt_clear_rt_table(rt_table_t *rt_table);
void rt_free_rt_table(rt_table_t *rt_table);
void rt_dump_rt_table(rt_table_t *rt_table);

// Inline function to remove a specific routing table entry
static inline void rt_entry_remove(rt_table_t *rt_table,
                                   rt_entry_t *rt_entry){
    // Handle corner cases and remove the entry from linked list
    if(!rt_entry->prev){
        if(rt_entry->next){
            rt_entry->next->prev = 0;
            rt_table->head = rt_entry->next;
            rt_entry->next = 0;
            return;
        }
		rt_table->head = NULL;
        return;
    }
    if(!rt_entry->next){
        rt_entry->prev->next = 0;
        rt_entry->prev = 0;
        return;
    }

    // Remove the entry from linked list
    rt_entry->prev->next = rt_entry->next;
    rt_entry->next->prev = rt_entry->prev;
    rt_entry->prev = 0;
    rt_entry->next = 0;
}

// Macros to simplify the iteration over the routing table
#define ITERTAE_RT_TABLE_BEGIN(rt_table_ptr, rt_entry_ptr)                \
{                                                                         \
    rt_entry_t *_next_rt_entry = 0;                                       \
    for((rt_entry_ptr) = (rt_table_ptr)->head;                            \
            (rt_entry_ptr);                                               \
            (rt_entry_ptr) = _next_rt_entry) {                            \
        _next_rt_entry = (rt_entry_ptr)->next;

#define ITERTAE_RT_TABLE_END(rt_table_ptr, rt_entry_ptr)  }}

#endif /* __RT__ */
