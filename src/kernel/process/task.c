/*
    describes the IA-32 architecture’s task management facilities.
     These facilities are only available when the processor is running in protected mode.
*/

#define KERNEL__Vir_MM
#define KERNEL__PAGE_MM
#include <i386types.h>
#include <init/video.h>
#include <mm.h>
#include <stddef.h>
#include <task.h>

static task_control_block_t main_task, *running_task, otherTask;

void yield()
{
    task_control_block_t* last = running_task;
    running_task = running_task->new_tasks;

    switch_to_task(&last->regs, &running_task->regs);
}

static void otherMain()
{
    kprintf(2, 11, "switch\n");
}
void init_multitasking()
{
    // Get EFLAGS and cr3

    asm volatile(
        "movl %%cr3, %%eax;"
        "movl %%eax, %0;"
        : "=m"(main_task.regs.cr3)::"%eax");

    asm volatile(
        "pushfl;"
        "pop %%eax;"
        "movl %%eax , %0;"
        : "=m"(main_task.regs.eflags)::"%eax");

    create_task(&otherTask, otherMain, main_task.regs.eflags, main_task.regs.cr3);

    main_task.new_tasks = &otherTask;
    otherTask.new_tasks = &main_task;

    running_task = &main_task;
}

void create_task(task_control_block_t* task, void (*task_func)(), uint32_t eflags, uint32_t cr3)
{
    task->regs.eax = 0;
    task->regs.ebx = 0;
    task->regs.ecx = 0;
    task->regs.edx = 0;
    task->regs.esi = 0;
    task->regs.edi = 0;

    task->regs.eflags = eflags;
    task->regs.eip = (uint32_t)task_func;
    task->regs.cr3 = (uint32_t)cr3;
    task->regs.esp = (uint32_t)alloc_page(2);
    task->new_tasks = 0;
}
void doIt()
{
    kprintf(2, 11, "Switching to otherTask... \n");
    yield();
    kprintf(2, 11, "Returned to mainTask!\n");
}