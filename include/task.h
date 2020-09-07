#ifndef _TASK_H_

#define _TASK_H_

#include <i386types.h>

typedef uint32_t pid_t;
/*Create thread data structure*/
// typedef struct thread {
// } thread_t;

/*Create process data strucuture*/
// typedef struct process {
// } process_t;

/*Create a task (task can be a process or a thread*/
typedef void* task_t;

// State of task
typedef enum state { running, ready, blocked, Nil } state_t;

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
    char task_name[50];
} __attribute__((packed)) process_control_block_t;

/*

    to implementing task model , kernel maintins table called
    task_table with one entry per process
*/
typedef struct task_table {
    process_control_block_t pcb_t;
    process_control_block_t* next_entry;
} __attribute__((packed)) task_table_t;
/*
    Create multitasking initialisation function
    create it to conserve task information in PCB
*/
void initialise_multitasking();

/*
    task states
*/

// task actually using by CPU
void task_running(task_t task_);

// task unable to run until some external event happens
void task_blocked(task_t task_);

// tasks runnable
void task_ready(task_t task_);

/*
    task transitions
*/

// task block for input
void task_running_blocked(task_t task_);

// sheduler picks another task
void running_ready(task_t task_);

// sheduler picks this process
void ready_running(task_t task_);

// input becomes available
void blocked_ready(task_t task_);

#endif // !_TASK_H_
