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
    bno085.enable_gyro_integrated_rotation_vector(50);
    ThisThread::sleep_for(5000ms);
    while (true) {
        if (data_available) {
            if (bno085.get_readings()) {
                float quatI = bno085.get_quat_i();
                float quatJ = bno085.get_quat_j();
                float quatK = bno085.get_quat_k();
                float quatReal = bno085.get_quat_real();
                float gyroX = bno085.get_fast_gyro_x();
                float gyroY = bno085.get_fast_gyro_y();
                float gyroZ = bno085.get_fast_gyro_z();

                printf("Quat I : %f\tQuat J : %f\tQuat K : %f\tQuatReal : %f\tGyro X: %f\tGyro Y : "
                       "%f\tGyro Z : %f\n",
                        quatI,
                        quatJ,
                        quatK,
                        quatReal,
                        gyroX,
                        gyroY,
                        gyroZ);
            }

        } else {
            ThisThread::sleep_for(1ms);
        }
    }
}
