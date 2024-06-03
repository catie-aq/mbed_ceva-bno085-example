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

int main()
{
    printf("\n\n\n\n=======================Init=============================\n\n\n\n");
    I2C i2c(I2C1_SDA, I2C1_SCL);
    BNO085 bno085(&i2c, 0x4B);
    bno085.initialize();
    while (true) {
        led1 = !led1;
        if (led1) {
            // printf("Alive!\n");
        }
        ThisThread::sleep_for(HALF_PERIOD);
    }
}
