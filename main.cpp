/*
 * Copyright (c) 2024, CATIE
 * SPDX-License-Identifier: Apache-2.0
 */
#include "bno085.h"
#include "mbed.h"

namespace {
#define HALF_PERIOD 500ms
}

static DigitalOut led1(LED1);
static InterruptIn wake_up(WKUP);
bool data_available = false;

void flip()
{
    led1 = !led1;
    data_available = true;
}

int main()
{
    printf("\n\n\n\n=======================Init=============================\n\n\n\n");
    wake_up.fall(flip);

    I2C i2c(I2C1_SDA, I2C1_SCL);
    BNO085 bno085(&i2c, 0x4B);
    bno085.initialize();
    bno085.enable_step_counter(500);
    ThisThread::sleep_for(1000ms);
    while (true) {
        if (data_available) {
            data_available = false;
            bno085.get_readings();
            uint16_t steps = bno085.get_step_count();
            printf("Steps : %u\n", steps);
            ThisThread::sleep_for(250ms);
        }
    }
}
