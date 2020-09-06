#ifndef _TASK_H_

#include "init/paging.h"
#include <stdint.h>

#define _TASK_H_

typedef uint32_t pid_t;
t
    /*Create thread data structure*/
    typedef struct thread {
} thread_t;

/*Create process data strucuture*/
typedef struct process {
} process_t;

/*Create a task (task can be a process or a thread*/
typedef void task_t;

// State of task
enum state_t { running, sleep, execute };

/*
    Proces control block
    data structure that will keep track of a task's information
*/
typedef struct process_control_block {
    intptr_t stack_;      // a field for the task's kernel stack top
    virtaddr_t virt_addr; // a field for the task's virtual address space
    task_t* new_tasks; // field that can be used for multiple different linked lists of tasks later on
    state_t state_task;
    pid_t process_id;
    char task_name[20];
} __attribute__((packed)) process_control_block__t;

/*

define
*/

/*
    Create multitasking initialisation function
    create it to conserve task information in PCB
*/
void initialise_multitasking();

#endif // !_TASK_H_
