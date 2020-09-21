#include <init/pic.h>
#include <init/pit.h>
#include <init/video.h>
#include <task.h>

extern int32_t system_timer_fractions, system_timer_ms, IRQ0_fractions, IRQ0_ms,
    IRQ0_frequency, PIT_reload_value;

extern sheduler_t sheduler;

uint32_t compteur = 0;
uint8_t frequency = 0;

void conserv_status_byte()
{
    uint8_t status = read_back_channel(PIT_0);
    compteur++;
    print_frequence(compteur % IRQ0_frequency);
    if (status != 0x34)
        Init_PIT(PIT_0, frequency);
}

void sheduler_cpu_timer()
{
    if (sheduler.init_timer == 1) {
        if (sheduler.task_timer == 0) {
            sheduler.task_timer = DELAY_PER_TASK;
            __switch();
        }
        else
            sheduler.task_timer--;
    }
}

void Init_PIT(int8_t channel, uint8_t frequence)
{
    frequency = frequence;
    calculate_frequency();
    pit_send_command(0b00110100 | (channel << 6));
    /* Reuint8_tger la valeur du compteur*/
    set_pit_count(channel, PIT_reload_value);
}

int8_t read_back_channel(int8_t channel)
{
    uint8_t command = 0x00;

    switch (channel) {
    case PIT_0:
        command |= READ_BACK_TIMER_0(1);
        break;
    case PIT_1:
        command |= READ_BACK_TIMER_1(1);
        break;

    case PIT_2:
        command |= READ_BACK_TIMER_2(1);
        break;
    default:
        break;
    }

    command |= LATCH_STATUS_FLAG(0) | LATCH_COUNT_FLAG(0) | READ_BACK_COMMAND;

    pit_send_command((uint8_t)command);

    return read_pit_count(PIT_0);
}