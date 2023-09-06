
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <assert.h>
#include <errno.h>
#include "timerlib.h"

// Function to convert timespec to milliseconds.
unsigned long timespec_to_millisec(struct timespec *time) {
    // Convert seconds to milliseconds and add nanoseconds converted to milliseconds
    return (time->tv_sec * 1000) + (time->tv_nsec / 1000000);
}

// Function to fill in timespec structure based on provided milliseconds.
void timer_fill_itimerspec(struct timespec *ts, unsigned long msec) {
    // Initialize the timespec structure to zero
    memset(ts, 0, sizeof(struct timespec));
    
    // If msec is zero, then simply return
    if(!msec) return;
    
    // Calculate seconds and nanoseconds from milliseconds
    ts->tv_sec = msec / 1000;
    ts->tv_nsec = (msec % 1000) * 1000000;
}

// Callback wrapper to handle timer state and user-defined callback
static void timer_callback_wrapper(union sigval arg) {
    Timer_t *timer = (Timer_t *)(arg.sival_ptr);
    if(timer->timer_state == TIMER_RESUMED) {
        if(timer->sec_exp_timer != 0) {
            timer->timer_state = TIMER_RUNNING;
        }
    }

    // Invoke the user-provided callback function
    timer->cb(timer, timer->user_arg);
}

// Function to set up and initialize the timer
Timer_t* setup_timer(
    void (*timer_cb)(Timer_t*, void *),     // User-defined callback function
    unsigned long exp_timer,                // Initial expiration time in milliseconds
    unsigned long sec_exp_timer,            // Subsequent expiration time in milliseconds
    uint32_t threshold,                     // Maximum number of expirations (0 for infinite)
    void *user_arg,                         // User argument for callback
    bool exponential_backoff) {             // Flag for exponential backoff

    // Allocate and initialize Timer_t structure
    Timer_t *timer = calloc(1, sizeof(Timer_t));
    timer->user_arg = user_arg;
    timer->exp_timer = exp_timer;
    timer->sec_exp_timer = sec_exp_timer;
    timer->cb = timer_cb;
    timer->thresdhold = threshold;
    timer_set_state(timer, TIMER_INIT);
    timer->exponential_backoff = exponential_backoff;
    
    // Sanity check to ensure that callback function is provided
    assert(timer->cb);
    
    struct sigevent evp;
    memset(&evp, 0, sizeof(struct sigevent));
    
    evp.sigev_value.sival_ptr = (void *)(timer);
    evp.sigev_notify = SIGEV_THREAD;
    evp.sigev_notify_function = timer_callback_wrapper;
    
    // Create the POSIX timer
    int rc = timer_create (CLOCK_REALTIME, &evp, &timer->posix_timer);
    assert(rc >= 0);
    
    // Fill in the timer specs for initial and subsequent intervals
    timer_fill_itimerspec(&timer->ts.it_value, timer->exp_timer);
    timer_fill_itimerspec(&timer->ts.it_interval, timer->sec_exp_timer);
    
    timer->exp_back_off_time = 0;
    return timer;
}

// Function to restart a paused or stopped timer
void resurrect_timer(Timer_t *timer) {
    int rc = timer_settime(timer->posix_timer, 0, &timer->ts, NULL);
    assert(rc >= 0);
}

// Function to start the timer
void start_timer(Timer_t *timer) {
    // Resurrect (or start) the timer
    resurrect_timer(timer);
    // Set the timer state to running
    timer_set_state(timer, TIMER_RUNNING);
}

// Function to get the remaining time on a timer in milliseconds
unsigned long timer_get_time_remaining_in_mill_sec(Timer_t *timer) {
    struct itimerspec remaining_time;
    memset(&remaining_time, 0, sizeof(struct itimerspec));
    
    timer_gettime(timer->posix_timer, &remaining_time);
    
    return timespec_to_millisec(&remaining_time.it_value);
}

// Function to pause the timer
void pause_timer(Timer_t *timer) {
    // Get the remaining time on the timer and store it
    timer->time_remaining = timer_get_time_remaining_in_mill_sec(timer);
    
    // Set the timer's specs to zero, effectively pausing it
    timer_fill_itimerspec(&timer->ts.it_value, 0);
    timer_fill_itimerspec(&timer->ts.it_interval, 0);
    
    // Update the timer specs to pause it
    resurrect_timer(timer);
    
    // Change the state of the timer to paused
    timer_set_state(timer, TIMER_PAUSED);
}

// Function to resume a paused timer
void resume_timer(Timer_t *timer) {
    // Make sure the timer is actually paused before attempting to resume it
    assert(timer_get_current_state(timer) == TIMER_PAUSED);
    
    // Reset the timer's specs based on the time remaining
    timer_fill_itimerspec(&timer->ts.it_value, timer->time_remaining);
    timer_fill_itimerspec(&timer->ts.it_interval, timer->sec_exp_timer);
    
    // Reset the time remaining to zero
    timer->time_remaining = 0;
    
    // Resurrect (or resume) the timer
    resurrect_timer(timer);
    
    // Set the state of the timer to resumed
    timer_set_state(timer, TIMER_RESUMED);
}
void delete_timer(Timer_t **timer) {
    timer_delete((*timer)->posix_timer);
    free(*timer);
    *timer = NULL;
}

// Function to cancel a timer
void cancel_timer(Timer_t *timer) {
    timer->ts.it_value.tv_sec = 0;
    timer->ts.it_value.tv_nsec = 0;
    timer->ts.it_interval.tv_sec = 0;
    timer->ts.it_interval.tv_nsec = 0;
    timer_settime(timer->posix_timer, 0, &timer->ts, NULL);
    timer_set_state(timer, TIMER_CANCELLED);
}

// Function to print the timer state
void print_timer_state(Timer_t *timer) {
    printf("Timer State: ");
    switch(timer->timer_state) {
        case TIMER_INIT: printf("INIT\n"); break;
        case TIMER_DELETED: printf("DELETED\n"); break;
        case TIMER_PAUSED: printf("PAUSED\n"); break;
        case TIMER_CANCELLED: printf("CANCELLED\n"); break;
        case TIMER_RESUMED: printf("RESUMED\n"); break;
        case TIMER_RUNNING: printf("RUNNING\n"); break;
        default: printf("UNKNOWN\n"); break;
    }
}


