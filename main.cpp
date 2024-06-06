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
    uint8_t activity_confidences[9];
    uint32_t enable_activities = 0xF1;
    bno085.enable_activity_classifier(500, enable_activities, activity_confidences);
    ThisThread::sleep_for(5000ms);
    while (true) {
        if (data_available) {
            if (bno085.get_readings()) {
                uint8_t activity_number = bno085.get_activity_classifier();
                if (activity_number == 0)
                    printf("Unknown\n");
                else if (activity_number == 1)
                    printf("In vehicle\n");
                else if (activity_number == 2)
                    printf("On bicycle\n");
                else if (activity_number == 3)
                    printf("On foot\n");
                else if (activity_number == 4)
                    printf("Still\n");
                else if (activity_number == 5)
                    printf("Tilting\n");
                else if (activity_number == 6)
                    printf("Walking\n");
                else if (activity_number == 7)
                    printf("Running\n");
                else if (activity_number == 8)
                    printf("On stairs\n");
            }
        }
    }
}
