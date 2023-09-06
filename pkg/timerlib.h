// Make sure the Timer Wrap is not already defined to avoid redefinition
#ifndef __TIMER_WRAP__  
#define __TIMER_WRAP__

// Include standard header files for time, signal handling, etc.
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

// Enumeration to represent the various states a timer can be in
typedef enum{
	TIMER_INIT,
	TIMER_DELETED,
	TIMER_PAUSED,
	TIMER_CANCELLED,
	TIMER_RESUMED,
	TIMER_RUNNING,
} TIMER_STATE_T;

// Timer structure definition
typedef struct Timer_{

    // Timer Configurations
    timer_t posix_timer;  // POSIX timer_t structure
    void *user_arg;  // Argument for the callback function
	unsigned long exp_timer;  // First expiration time in milliseconds
	unsigned long sec_exp_timer;  // Subsequent expiration time in milliseconds
	uint32_t thresdhold;  // Number of times to invoke the timer callback
	void (*cb)(struct Timer_ *, void *);  // Timer callback function
	bool exponential_backoff;  // Flag to indicate whether the timer uses exponential backoff

	// Dynamic attributes of the timer
	unsigned long time_remaining;  // Time left for the next expiration when paused
	uint32_t invocation_counter;  // Counter to keep track of the number of invocations
	struct itimerspec ts;  // POSIX interval timer specification
	unsigned long exp_back_off_time;  // Time for exponential back-off
	TIMER_STATE_T timer_state;  // Current state of the timer
} Timer_t;



static inline TIMER_STATE_T timer_get_current_state(Timer_t *timer);
static inline void timer_set_state(Timer_t *timer, TIMER_STATE_T timer_state);
void resurrect_timer(Timer_t *timer);
void restart_timer(Timer_t *timer); // Added
void reschedule_timer(Timer_t *timer, unsigned long new_exp_timer, unsigned long new_sec_exp_timer); // Added

void start_timer(Timer_t *timer);
unsigned long timer_get_time_remaining_in_mill_sec(Timer_t *timer);
void pause_timer(Timer_t *timer);
void resume_timer(Timer_t *timer);
void delete_timer(Timer_t **timer);
void cancel_timer(Timer_t *timer);
void print_timer_state(Timer_t *timer);
unsigned long timespec_to_millisec(struct timespec *time);
void timer_fill_itimerspec(struct timespec *ts, unsigned long msec);



// Function prototype for setting up the timer
Timer_t*
setup_timer(
    void (*timer_cb)(Timer_t*, void *),  // Timer Callback with user data
    unsigned long exp_timer,  // First expiration time interval in msec
    unsigned long sec_exp_timer,  // Subsequent expiration time interval in msec
    uint32_t threshold,  // Max number of expirations, 0 for infinite
    void *user_arg,  // Argument to timer callback
    bool exponential_backoff);  // Is Timer using Exponential Back-off

// Inline function to get the current state of the timer
static inline TIMER_STATE_T
timer_get_current_state(Timer_t *timer){
    return timer->timer_state;
}

// Inline function to set the state of the timer
static inline void
timer_set_state(Timer_t *timer, TIMER_STATE_T timer_state){
    timer->timer_state = timer_state;
}

// Function prototype for resurrecting a paused or cancelled timer
void
resurrect_timer(Timer_t *timer);

// Function prototype for starting a timer
void
start_timer(Timer_t *timer);

#endif /* __TIMER_WRAP__  */
