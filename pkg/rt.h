// Include guard to prevent double inclusion
#ifndef __RT__
#define __RT__

// Standard includes for fixed-size integers and booleans
#include <stdint.h>
#include <stdbool.h>

// Custom library for time-related functionality
#include "timerlib.h"

// Time for a routing table entry to expire, set to 30 seconds
#define RT_TABLE_EXP_TIME 30

// Data structure representing the keys in a routing table entry
typedef struct rt_entry_keys_{
    char dest[16]; // Destination IP address
    char mask;     // Subnet mask
} rt_entry_keys_t;

// Data structure representing a routing table entry
typedef struct rt_entry_{
    rt_entry_keys_t rt_entry_keys; // Keys for the entry (destination and mask)
    char gw_ip[16];                // Gateway IP address
    char oif[32];                  // Output interface
    uint32_t time_to_expire;       // Time left before the entry expires
    struct rt_entry_ *prev;        // Pointer to the previous entry in the list
    struct rt_entry_ *next;        // Pointer to the next entry in the list
} rt_entry_t;

// Data structure for the routing table
typedef struct rt_table_{
    rt_entry_t *head; // Head of the linked list of routing table entries
} rt_table_t;

// Function prototypes for routing table management
void rt_init_rt_table(rt_table_t *rt_table);
bool rt_add_new_rt_entry(rt_table_t *rt_table, char *dest_ip, char mask, char *gw_ip, char *oif);
bool rt_delete_rt_entry(rt_table_t *rt_table, char *dest_ip, char mask);
bool rt_update_rt_entry(rt_table_t *rt_table, char *dest_ip, char mask, char *new_gw_ip, char *new_oif);
void rt_clear_rt_table(rt_table_t *rt_table);
void rt_free_rt_table(rt_table_t *rt_table);
void rt_dump_rt_table(rt_table_t *rt_table);

// Inline function to remove an entry from the routing table
static inline void rt_entry_remove(rt_table_t *rt_table, rt_entry_t *rt_entry){
    // Remove the entry here
    // Logic varies depending on whether entry is at head, tail, or middle
}

// Macros to simplify iterating over the routing table
#define ITERATE_RT_TABLE_BEGIN(rt_table_ptr, rt_entry_ptr)                \
{                                                                         \
    // Initialization logic for iterating over the routing table
}

#define ITERATE_RT_TABLE_END(rt_table_ptr, rt_entry_ptr)                  \
{                                                                         \
    // Cleanup logic for iterating over the routing table
}

#endif // __RT__
