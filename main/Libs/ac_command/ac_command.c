//
// Created by nikita on 20.05.24.
//

#include "ac_command.h"
#include "commands_list.h"

static int current_mode = 0;
static float current_temperature = 21;
static uint32_t* data = cooling_temperatures[11];

uint32_t* get_data()
{
    return data;
}

int get_data_length(void)
{
    return COMMAND_LENGTH;
}

void set_temperature(float temperature)
{
    current_temperature = temperature;
    int temperature_index = (int)temperature - 10;

    switch (current_mode) {
        case 0:
            data = power_off;
            break;
        case 1:
            data = heating_temperature[temperature_index];
            break;
        case 2:
            data = cooling_temperatures[temperature_index];
            break;
        case 3:
            data = auto_temperature[temperature_index];
            break;
    }
}

void set_mode(int mode) {
    current_mode = mode;
    set_temperature(current_temperature);
}
