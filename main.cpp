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
    bno085.enable_rotation_vector(50);
    ThisThread::sleep_for(1000ms);
    while (true) {
        if (data_available) {
            data_available = false;
            bno085.get_readings();
            float quat_i = bno085.get_quat_i();
            float quat_j = bno085.get_quat_j();
            float quat_k = bno085.get_quat_k();
            float quat_real = bno085.get_quat_real();
            float quat_radian_accuracy = bno085.get_quat_radian_accuracy();
            printf("Quat i : \t%f\tQuat j : \t%f\tQuat k : \t%f\tQuat real : \t%f\tQuat radian "
                   "accuracy : \t%f\n ",
                    quat_i,
                    quat_j,
                    quat_k,
                    quat_real,
                    quat_radian_accuracy);
            ThisThread::sleep_for(25ms);
        }
    }
}
