#include "opt-synchprobs.h"
#include "counter.h"
#include <types.h>
#include <kern/errno.h>
#include <lib.h>
#include <test.h>
#include <thread.h>
#include <synch.h>


/*
 * Declare the counter variable that all threads increment or decrement
 * via the interface provided here.
 *
 * Declaring it "volatile" instructs the compiler to always (re)read the
 * variable from memory and not to optimise by keeping the value in a process 
 * register and avoid memory references.
 *
 * NOTE: The volatile declaration is actually not needed for the provided code 
 * as the variable is only loaded once in each function.
 */

static volatile int the_counter;


struct semaphore *counter_mutex;


void counter_increment(void)
{
        P(counter_mutex);
        the_counter += 1;
        V(counter_mutex);
}

void counter_decrement(void)
{
        P(counter_mutex);
        the_counter -= 1;
        V(counter_mutex);
}

int counter_initialise(int val)
{
        counter_mutex = sem_create("counter", 1);
        if (counter_mutex == NULL) return ENOMEM;
        P(counter_mutex);
        the_counter = val;
        V(counter_mutex);
        return 0;
}

int counter_read_and_destroy(void)
{
        P(counter_mutex);
        int sto = the_counter;
        sem_destroy(counter_mutex);
        return sto;
}
