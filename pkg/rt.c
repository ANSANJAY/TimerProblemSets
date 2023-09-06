// Included header files for standard library and custom routing table
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <assert.h>
#include "rt.h"

// Initialize routing table, setting its head to NULL
void rt_init_rt_table(rt_table_t *rt_table){
    rt_table->head = NULL;
}

// Add a new routing table entry
bool rt_add_new_rt_entry(rt_table_t *rt_table,
                         char *dest,
                         char mask,
                         char *gw_ip,
                         char *oif,
						 void (*delete_cbk)(Timer_t*, void *)) {

    rt_entry_t *head = NULL;
    rt_entry_t *rt_entry = NULL;

    // Allocate memory for a new routing entry
    rt_entry = calloc(1, sizeof(rt_entry_t));

    // Return false if memory allocation failed
    if(!rt_entry)
        return false;

    // Populate routing entry fields
    strncpy(rt_entry->rt_entry_keys.dest, dest, sizeof(rt_entry->rt_entry_keys.dest));
    rt_entry->rt_entry_keys.mask = mask;

    if(gw_ip)
        strncpy(rt_entry->gw_ip, gw_ip, sizeof(rt_entry->gw_ip));
    if(oif)
        strncpy(rt_entry->oif, oif, sizeof(rt_entry->oif));

    // Set expiration time and timer
	rt_entry->time_to_expire = RT_TABLE_EXP_TIME;
	rt_entry->exp_timer = setup_timer(delete_cbk,
									  rt_entry->time_to_expire * 1000,
									  0,
									  0,
									  rt_entry,
									  false);

    // Add the new entry to the table
    head = rt_table->head;
    rt_table->head = rt_entry;
    rt_entry->prev = 0;
    rt_entry->next = head;
    if(head)
        head->prev = rt_entry;

    // Start the expiration timer
	start_timer(rt_entry->exp_timer);
    return true;
}

// Delete a specific routing table entry
bool rt_delete_rt_entry(rt_table_t *rt_table,
                        char *dest, char mask) {
    rt_entry_t *rt_entry = NULL;

    // Check if dest and mask are valid
	assert(dest && mask);

    // Iterate through the routing table to find and delete the entry
    ITERTAE_RT_TABLE_BEGIN(rt_table, rt_entry){
        if(strncmp(rt_entry->rt_entry_keys.dest, 
            dest, sizeof(rt_entry->rt_entry_keys.dest)) == 0 &&
            rt_entry->rt_entry_keys.mask == mask) {

            rt_entry_remove(rt_table, rt_entry);
			
            // Debug print for deleted entry
			printf("deleting rt entry %p [%s:%d]\n",
			       rt_entry, rt_entry->rt_entry_keys.dest,
			       rt_entry->rt_entry_keys.mask);

            free(rt_entry);
            return true;
        }
    } ITERTAE_RT_TABLE_END(rt_table, curr);

    return false;
}


// Dump the content of the routing table to stdout
void rt_dump_rt_table(rt_table_t *rt_table) {

    rt_entry_t *rt_entry = NULL;

    // Iterate through the routing table and print each entry
    ITERTAE_RT_TABLE_BEGIN(rt_table, rt_entry){

        printf("%-20s %-4d %-20s %-12s %usec (%lums)\n",
               rt_entry->rt_entry_keys.dest,
               rt_entry->rt_entry_keys.mask,
               rt_entry->gw_ip,
               rt_entry->oif,
			   rt_entry->time_to_expire,
			   timer_get_time_remaining_in_mill_sec(rt_entry->exp_timer));
    } ITERTAE_RT_TABLE_END(rt_tabl, rt_entry);
}
