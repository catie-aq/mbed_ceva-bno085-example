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
    bno085.enable_stability_classifier(5000);
    bno085.calibrate_gyro();
    printf("Stability Classifier enable");
    ThisThread::sleep_for(5000ms);
    while (true) {
        if (data_available) {
            bno085.get_readings();
            data_available -= 2;
            switch (bno085.get_stability_classifier()) {
                case 0:
                    printf("Unknown classification\n");
                    break;
                case 1:
                    printf("On table\n");
                    break;
                case 2:
                    printf("Stationary\n");
                    break;
                case 3:
                    printf("Stable\n");
                    break;
                case 4:
                    printf("Motion\n");
                    break;
                case 5:
                    printf("[Reserved]\n");
                    break;

                default:
                    ThisThread::sleep_for(1ms);
                    break;
            }
        } else {
            ThisThread::sleep_for(1ms);
        }
    }
}
