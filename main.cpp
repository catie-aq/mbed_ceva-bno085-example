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
    bno085.enable_magnetometer(50);
    ThisThread::sleep_for(1000ms);
    while (true) {
        if (data_available) {
            data_available = false;
            bno085.get_readings();
            float x = bno085.get_mag_x();
            float y = bno085.get_mag_y();
            float z = bno085.get_mag_z();
            uint8_t accuracy = bno085.get_mag_accuracy();
            printf("x : %fuT\t y : %fuT\t z : %fuT\t z : %d\n", x, y, z, accuracy);
            ThisThread::sleep_for(25ms);
        }
    }
}
