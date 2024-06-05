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
    ThisThread::sleep_for(1000ms);

    printf("========== For accelerometer ==========\n");
    float get_acce_range = bno085.get_range(FRS_RECORDID_ROTATION_VECTOR);
    printf("Range : \t%f\n", get_acce_range);
    float get_acce_resol = bno085.get_resolution(FRS_RECORDID_ROTATION_VECTOR);
    printf("Resolution : \t%f\n", get_acce_resol);
    int16_t get_acce_q_1 = bno085.get_q_1(FRS_RECORDID_ROTATION_VECTOR);
    printf("Q1 : \t%d\n", get_acce_q_1);
    int16_t get_acce_q_2 = bno085.get_q_2(FRS_RECORDID_ROTATION_VECTOR);
    printf("Q2 : \t%d\n", get_acce_q_2);
    int16_t get_acce_q_3 = bno085.get_q_3(FRS_RECORDID_ROTATION_VECTOR);
    printf("Q3 : \t%d\n", get_acce_q_3);
    printf("=======================================\n\n\n");
    while (state) {
        uint32_t rot_vector_power = bno085.read_frs_word(FRS_RECORDID_ROTATION_VECTOR, 3) & 0xFFFF;
        float rot_power = bno085.q_to_float(rot_vector_power, 10);
        printf("Rotation vector power : %f mA \n", rot_power);
        if (rot_power > 0) {
            state = false;
        }
    }
}
