// Include the necessary headers
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include "rt.h"  // Custom header for routing table functionality

// Declare a static routing table object
static rt_table_t rt;

// Function to delete a routing table entry when the timer expires
static void
rt_entry_delete_on_timer_expiry(Timer_t *timer, void *user_data){

    // Convert user_data back to rt_entry_t type
	rt_entry_t *rt_entry = (rt_entry_t *)user_data;

    // Delete the timer and free its memory
	delete_timer(rt_entry->exp_timer);
	free(rt_entry->exp_timer);
	rt_entry->exp_timer = NULL;

    // Delete the routing table entry and assert if it fails
	assert(rt_delete_rt_entry(&rt, rt_entry->rt_entry_keys.dest, 
								   rt_entry->rt_entry_keys.mask));
}

// Main function
int
main(int argc, char **argv){

    // Initialize the routing table
	rt_init_rt_table(&rt);

    // Add some initial routing entries
	rt_add_new_rt_entry(&rt, "122.1.1.1", 32, "10.1.1.1", "eth0", rt_entry_delete_on_timer_expiry);
    rt_add_new_rt_entry(&rt, "122.1.1.2", 32, "10.1.1.2", "eth1", rt_entry_delete_on_timer_expiry);
    rt_add_new_rt_entry(&rt, "122.1.1.3", 32, "10.1.1.3", "eth2", rt_entry_delete_on_timer_expiry);
    rt_add_new_rt_entry(&rt, "122.1.1.4", 32, "10.1.1.4", "eth3", rt_entry_delete_on_timer_expiry);
    rt_add_new_rt_entry(&rt, "122.1.1.5", 32, "10.1.1.5", "eth4", rt_entry_delete_on_timer_expiry);

    // Infinite loop for the menu
	while(1){
		printf("1. Add rt table entry\n");
		printf("3. Delete rt table entry\n");
		printf("4. Dump rt table\n");

        // Take the user's choice
		int choice;
		printf("Enter Choice :");
		scanf("%d", &choice);
		fflush(stdin);

        // Process the choice
        switch(choice){
            case 1:
                {
                    char dest[16];
                    uint8_t mask;
                    char oif[32];
                    char gw[16];
                    printf("Enter Destination :");
                    scanf("%s", dest);
                    printf("Mask : ");
                    scanf("%hhd", &mask);
                    printf("Enter oif name :");
                    scanf("%s", oif);
                    printf("Enter Gateway IP :");
                    scanf("%s", gw);

                    // Add the new routing table entry
                    if(!rt_add_new_rt_entry(&rt, dest, mask,
                        gw, oif, rt_entry_delete_on_timer_expiry)){
                        printf("Error : Could not add an entry\n");
                    }
				}
				break;
			case 4:
				// Dump the current routing table
				rt_dump_rt_table(&rt);
			default:
				// Do nothing for other inputs
				;
		}
	}
	return 0;
}
