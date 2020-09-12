#define KERNEL__Vir_MM
#include <i386types.h>
#include <mm.h>
#include <stddef.h>
#include <task.h>

task_control_block_t CURRENT_TASK;
uint32_t size_tcb = sizeof(CURRENT_TASK);
int i = 0;

void create_task(task_control_block_t* task, void* test, uint32_t stack, uint32_t size)
{
    task->stack_ = stack;
    task->process_id = 1;
    task->stack_size = 0xff;
    task->virt_addr = (virtaddr_t)test;
    task->state_task = ready;

    CURRENT_TASK.new_tasks = task;
}

void* test()
{
    i++;
}

void initialise_multitasking()
{
    uint32_t esp = 0;
    esp = (uint32_t)kmalloc(0xff);
    task_control_block_t firts_tcb, *new_task;

    firts_tcb.stack_size = 1024;
    firts_tcb.stack_ = esp + firts_tcb.stack_size;

    firts_tcb.virt_addr = (virtaddr_t)initialise_multitasking;
    firts_tcb.new_tasks = running;
    firts_tcb.process_id = 0;

    CURRENT_TASK = firts_tcb;

    create_task(new_task, test, (uint32_t)kmalloc(0xff), 22);

    switch_to_task();
}

/*

    Next write a function t create a new kernel task

    and put values on the new kernel stack to match the values that your "switch_to_task(task)"
    function expects to pop off the stack after switching to the task
*/