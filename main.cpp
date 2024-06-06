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
                unsigned long time_stamp = bno085.get_time_stamp();
                float QuatI = bno085.get_quat_i();
                float QuatJ = bno085.get_quat_j();
                float QuatK = bno085.get_quat_k();
                float QuatReal = bno085.get_quat_real();

                printf("[%u]Pitch : %f\tRoll : %f\tYaw: %f\n",
                        time_stamp,
                        QuatI,
                        QuatJ,
                        QuatK,
                        QuatReal);
            } else {
                ThisThread::sleep_for(1ms);
            }
        }
    }
}
