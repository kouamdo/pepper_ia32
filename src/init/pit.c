#include "../../include/init/pit.h"
#include "../../include/init/pic.h"
#include "../../include/init/video.h"

uint32_t compteur = 0;

void conserv_status_byte()
{
    uint8_t status = read_back_channel(PIT_0);
    compteur++;
    print_frequence(compteur % IRQ0_frequency);
    if (status != 0x34)
        Init_PIT(PIT_0, frequency);
}

void Init_PIT(int8_t channel, uint32_t frequence)
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
