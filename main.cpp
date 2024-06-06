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
    bno085.enable_gravity(50);
    ThisThread::sleep_for(5000ms);
    while (true) {
        if (data_available) {
            if (bno085.get_readings()) {
                float gravityX = bno085.get_gravity_x();
                float gravityY = bno085.get_gravity_y();
                float gravityZ = bno085.get_gravity_z();
                float gravityAccuracy = bno085.get_gravity_accuracy();
                printf("Gravity X : %f\tGravity Y : %f\tGravity Z : %f\tAccuracy :%f\n",
                        gravityX,
                        gravityY,
                        gravityZ,
                        gravityAccuracy);
            } else {
                ThisThread::sleep_for(1ms);
            }
        }
    }
}
