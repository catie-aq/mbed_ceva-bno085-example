/*
 * Copyright (c) 2024, CATIE
 * SPDX-License-Identifier: Apache-2.0
 */
#include "bno085.h"
#include "mbed.h"
const float pi = 3.14159265359;

namespace {
#define HALF_PERIOD 500ms
}

static DigitalOut led1(LED1);
static InterruptIn wake_up(WKUP);
bool data_available = false;
bool state = true;

void flipe()
{
    led1 = !led1;
    data_available++;
}

int main()
{
    printf("\n\n\n\n=======================Init=============================\n\n\n\n");

    I2C i2c(I2C1_SDA, I2C1_SCL);
    BNO085 bno085(&i2c, 0x4B);
    bno085.initialize();
    wake_up.fall(flipe);
    bno085.enable_rotation_vector(50);
    ThisThread::sleep_for(5000ms);
    while (true) {
        if (data_available) {
            if (bno085.get_readings()) {
                float pitch = bno085.get_pitch() * 180 / pi;
                float roll = bno085.get_roll() * 180 / pi;
                float yaw = bno085.get_yaw() * 180 / pi;
                printf("Pitch : %f\tRoll : %f\tYaw: %f\n", pitch, roll, yaw);
            } else {
                ThisThread::sleep_for(1ms);
            }
        }
    }
}
