
/*
 * Copyright (c) 2024, CATIE
 * SPDX-License-Identifier: Apache-2.0
 */
#include "bno085.h"
#include "mbed.h"

namespace {
#define HALF_PERIOD 500ms
}

int main()
{
    printf("\n\n\n\n=======================Init=============================\n\n\n\n");

    I2C i2c(I2C1_SDA, I2C1_SCL);
    BNO085 bno085(&i2c, 0x4B);
    bno085.initialize();
    bno085.enable_accelerometer(10);

    ThisThread::sleep_for(1000ms);
    while (true) {
        if (bno085.get_readings() != 0) {
            // uint16_t report_id = bno085.get_readings();
            // data_available = data_available - 2;
            float accx = bno085.get_accelX();
            float accy = bno085.get_accelY();
            float accz = bno085.get_accelZ();
            uint8_t lin_accuracy = bno085.get_accel_accuracy();
            printf("[] x : %f\t ,y : %f\t, z : %f\t, Accuracy : %u\n",
                    accx,
                    accy,
                    accz,
                    lin_accuracy);
        } else {
            // printf("Test\n");
            // data_available = 0;
            ThisThread::sleep_for(1ms);
        }
    }
}
