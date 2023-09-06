#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include "rt.h"  // Include the header file that defines the routing table structure and other functionalities

// Initialize the routing table by setting its head to NULL
void rt_init_rt_table(rt_table_t *rt_table){
    rt_table->head = NULL;
}

// Add a new entry to the routing table
bool rt_add_new_rt_entry(rt_table_t *rt_table,
                    char *dest, 
                    char mask, 
                    char *gw_ip, 
                    char *oif){

    rt_entry_t *head = NULL;
    rt_entry_t *rt_entry = NULL;

    // Allocate memory for a new routing table entry
    rt_entry = calloc(1, sizeof(rt_entry_t));

    // Check for memory allocation failure
    if(!rt_entry)
        return false;

    // Populate the new entry's fields
    strncpy(rt_entry->rt_entry_keys.dest, dest, sizeof(rt_entry->rt_entry_keys.dest));
    rt_entry->rt_entry_keys.mask = mask;

    if(gw_ip)
        strncpy(rt_entry->gw_ip, gw_ip, sizeof(rt_entry->gw_ip));
    if(oif)
        strncpy(rt_entry->oif, oif, sizeof(rt_entry->oif));

    // Set time to expire
	rt_entry->time_to_expire = RT_TABLE_EXP_TIME;

    // Add the new entry at the beginning of the linked list
    head = rt_table->head;
    rt_table->head = rt_entry;
    rt_entry->prev = 0;
    rt_entry->next = head;
    if(head)
        head->prev = rt_entry;
    
    return true;
}

// Delete an entry from the routing table
bool rt_delete_rt_entry(rt_table_t *rt_table,
    char *dest, char mask){

    rt_entry_t *rt_entry = NULL;

    // Iterate over the routing table to find the entry to delete
    ITERATE_RT_TABLE_BEGIN(rt_table, rt_entry){
    
        if(strncmp(rt_entry->rt_entry_keys.dest, 
            dest, sizeof(rt_entry->rt_entry_keys.dest)) == 0 &&
            rt_entry->rt_entry_keys.mask == mask){

            // Remove and free the entry
            rt_entry_remove(rt_table, rt_entry);
            free(rt_entry);
            return true;
        }
    } ITERATE_RT_TABLE_END(rt_table, curr);  // End of iteration macro

    return false;
}

// Update an existing entry in the routing table
// (Implementation missing)
bool rt_update_rt_entry(rt_table_t *rt_table,
                  char *dest, 
                  char mask, 
                  char *new_gw_ip, 
                  char *new_oif){
    return true;
}

// Clear all entries in the routing table
// (Implementation missing)
void rt_clear_rt_table(rt_table_t *rt_table){
}

// Free all memory associated with the routing table
// (Implementation missing)
void rt_free_rt_table(rt_table_t *rt_table){
}

// Dump the current state of the routing table to stdout
void rt_dump_rt_table(rt_table_t *rt_table){

    rt_entry_t *rt_entry = NULL;

    // Iterate over the routing table and print each entry
    ITERATE_RT_TABLE_BEGIN(rt_table, rt_entry){

        printf("%-20s %-4d %-20s %-12s %usec\n",
            rt_entry->rt_entry_keys.dest, 
            rt_entry->rt_entry_keys.mask, 
            rt_entry->gw_ip,
            rt_entry->oif,
			rt_entry->time_to_expire);
    } ITERATE_RT_TABLE_END(rt_table, rt_entry);  // End of iteration macro
}
