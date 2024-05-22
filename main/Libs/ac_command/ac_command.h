//
// Created by nikita on 20.05.24.
//

#ifndef LIGHTBULB_AC_COMMAND_H
#define LIGHTBULB_AC_COMMAND_H

#include <stdio.h>

uint32_t* get_data(void);
int get_data_length(void);

void set_temperature(float temperature);
void set_mode(int mode);

#endif //LIGHTBULB_AC_COMMAND_H
