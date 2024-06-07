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

DigitalIn button(BUTTON1);
static DigitalOut led1(LED1);
static InterruptIn wake_up(WKUP);
uint8_t data_available = false;
bool state = true;
uint8_t tare_counter = 0;

void flipe()
{
    led1 = !led1;
    if (data_available < 253) {
        data_available++;
    }
}

int main()
{
    printf("\n\n\n\n=======================Init=============================\n\n\n\n");

    I2C i2c(I2C1_SDA, I2C1_SCL);
    BNO085 bno085(&i2c, 0x4B);
    bno085.initialize();
    wake_up.fall(flipe);
    bno085.enable_rotation_vector(100);
    ThisThread::sleep_for(5000ms);
    while (true) {
        if (data_available) {
            if (bno085.get_readings()) {
                tare_counter++; // each 100ms increments tare_counter
                unsigned long time_stamp = bno085.get_time_stamp();
                float QuatI = bno085.get_quat_i() * 180 / pi;
                float QuatJ = bno085.get_quat_j() * 180 / pi;
                float QuatK = bno085.get_quat_k() * 180 / pi;
                float QuatReal = bno085.get_quat_real();

                printf("[%u] Pitch : %f\tRoll : %f\tYaw: %f\n",
                        time_stamp,
                        QuatI,
                        QuatJ,
                        QuatK,
                        QuatReal);
                if (tare_counter > 100) {
                    tare_counter = 0;
                    if (state) {
                        printf("Tare in progress\n");
                        led1 = 1;
                        ThisThread::sleep_for(1000ms);
                        bno085.tare_now(); // Tare the rotation vector
                    }
                    state = !state;
                    if (state) {
                        printf("\nTare Save\n");
                        bno085.save_tare(); // Save Actual Tare
                    }
                }
                if (button.read()) {
                    bno085.clear_tare(); // return at the first Tare (is not modifiable)
                }

            } else {
                ThisThread::sleep_for(1ms);
            }
        }
    }
}
