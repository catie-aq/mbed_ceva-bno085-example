/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "bno085.h"
#include "lwshell/lwshell.h"
#include "lwshell_opts.h"
#include "mbed.h"
// Blinking rate in milliseconds
#define BLINKING_RATE 500ms

static DigitalOut led(LED1);

static UnbufferedSerial serial_port(CONSOLE_TX, CONSOLE_RX);
I2C i2c(I2C1_SDA, I2C1_SCL);
BNO085 bno085(&i2c, 0x4B);
EventQueue queue(32 * EVENTS_EVENT_SIZE);
bool state;
const float pi = 3.14159265359;
DigitalIn button(BUTTON1);
static DigitalOut led1(LED1);
bool state1 = true;
uint8_t tare_counter = 0;

void bno_accelerometer_data()
{
    bno085.enable_accelerometer(5);
    while (state) {
        if (bno085.get_available_data()) {
            bno085.get_readings();
            uint8_t IdReport = bno085.get_readings();
            float x_acc = bno085.get_accel_x();
            float y_acc = bno085.get_accel_y();
            float z_acc = bno085.get_accel_z();
            printf("[%02X] accel x :\t%f\taccel y :\t%f\t accel  :\t%f\n",
                    IdReport,
                    x_acc,
                    y_acc,
                    z_acc);
        } else {
            ThisThread::sleep_for(1ms);
        }
    }
}

void bno_linear_accelerometer_data()
{
    bno085.enable_linear_accelerometer(5);
    while (state) {
        if (bno085.get_available_data()) {
            bno085.get_readings();
            float accx = bno085.get_lin_accel_x();
            float accy = bno085.get_lin_accel_y();
            float accz = bno085.get_lin_accel_z();
            uint8_t lin_accuracy = bno085.get_lin_accel_accuracy();
            printf("x : %f\t ,y : %f\t, z : %f\t, Accuracy : %u\n", accx, accy, accz, lin_accuracy);
        }
    }
}

void bno_rotation_vector_data()
{
    bno085.enable_rotation_vector(50);
    while (state) {
        if (bno085.get_available_data()) {
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
        }
    }
}

void bno_gyroscope_data()
{
    bno085.enable_gyro(50);
    while (state) {
        if (bno085.get_available_data()) {
            bno085.get_readings();
            float x = bno085.get_gyro_x();
            float y = bno085.get_gyro_y();
            float z = bno085.get_gyro_z();
            printf("x : %f\t y : %f\t z : %f\n", x, y, z);
        }
    }
}

void bno_magnetometer_data()
{
    bno085.enable_magnetometer(50);
    while (state) {
        if (bno085.get_available_data()) {
            bno085.get_readings();
            float x = bno085.get_mag_x();
            float y = bno085.get_mag_y();
            float z = bno085.get_mag_z();
            uint8_t accuracy = bno085.get_mag_accuracy();
            printf("x : %fuT\t y : %fuT\t z : %fuT\t z : %d\n", x, y, z, accuracy);
        }
    }
}

void bno_step_counter_data()
{
    bno085.enable_step_counter(500);
    while (state) {
        if (bno085.get_available_data()) {
            bno085.get_readings();
            uint16_t steps = bno085.get_step_count();
            printf("Steps : %u\n", steps);
        }
    }
}

void bno_calibration_data()
{
    bno085.enable_stability_classifier(500);
    bno085.calibrate_gyro();
    while (state) {
        if (bno085.get_available_data()) {
            bno085.get_readings();
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
        }
    }
}

void bno_activity_classifier_data()
{
    uint8_t activity_confidences[9];
    uint32_t enable_activities = 0xF1;
    bno085.enable_activity_classifier(500, enable_activities, activity_confidences);
    while (state) {
        if (bno085.get_available_data()) {
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

void bno_fast_gyroscope_data()
{
    bno085.enable_gyro_integrated_rotation_vector(50);
    while (state) {
        if (bno085.get_available_data()) {
            bno085.get_readings();
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
    }
}

void bno_gravity_data()
{
    bno085.enable_gravity(50);
    while (state) {
        if (bno085.get_available_data()) {
            bno085.get_readings();
            float gravityX = bno085.get_gravity_x();
            float gravityY = bno085.get_gravity_y();
            float gravityZ = bno085.get_gravity_z();
            float gravityAccuracy = bno085.get_gravity_accuracy();
            printf("Gravity X : %f\tGravity Y : %f\tGravity Z : %f\tAccuracy :%f\n",
                    gravityX,
                    gravityY,
                    gravityZ,
                    gravityAccuracy);
        }
    }
}

void bno_angles_y_p_r_data()
{
    bno085.enable_rotation_vector(50);
    while (state) {
        if (bno085.get_available_data()) {
            if (bno085.get_readings()) {
                float pitch = bno085.get_pitch() * 180 / pi;
                float roll = bno085.get_roll() * 180 / pi;
                float yaw = bno085.get_yaw() * 180 / pi;
                printf("Pitch : %f\tRoll : %f\tYaw: %f\n", pitch, roll, yaw);
            }
        }
    }
}

void bno_timestamp_data()
{
    bno085.enable_rotation_vector(50);
    while (state) {
        if (bno085.get_available_data()) {
            if (bno085.get_readings()) {
                unsigned long time_stamp = bno085.get_time_stamp();
                float pitch = bno085.get_pitch() * 180 / pi;
                float roll = bno085.get_roll() * 180 / pi;
                float yaw = bno085.get_yaw() * 180 / pi;
                printf("[%u] Pitch : %f\tRoll : %f\tYaw: %f\n", time_stamp, pitch, roll, yaw);
            }
        }
    }
}

void bno_tare_rotation_vector_data()
{
    bno085.enable_rotation_vector(50);
    while (state) {
        if (bno085.get_available_data()) {
            if (bno085.get_readings()) {
                tare_counter++; // each 50ms increments tare_counter
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
                if (tare_counter > 200) {
                    tare_counter = 0;
                    if (state1) {
                        printf("Tare in progress\n");
                        led1 = 1;
                        ThisThread::sleep_for(1000ms);
                        bno085.tare_now(); // Tare the rotation vector
                    }
                    state1 = !state1;
                    if (state1) {
                        printf("\nTare Save\n");
                        bno085.save_tare(); // Save Actual Tare
                    }
                }
                if (button.read()) {
                    bno085.clear_tare(); // return to the initial Tare (not modifiable)
                }
            }
        }
    }
}

void bno085_soft_reset()
{
    bno085.soft_reset();
}

int32_t _do_led(int32_t argc, char **argv)
{
    if (strcmp(argv[1], "on") == 0) {
        led = 1;
    } else if (strcmp(argv[1], "off") == 0) {
        led = 0;
    } else {
        char *str = "on to trun on or off to turn off\n";
        serial_port.write(str, strlen(str));
    }

    return 0;
}

int32_t _get_accelerometer_data(int32_t argc, char **argv)
{
    if (strcmp(argv[1], "on") == 0) {
        state = true;
        queue.call(bno_accelerometer_data);
        led = 1;
    } else if (strcmp(argv[1], "off") == 0) {
        led = 0;
        state = false;
        queue.call(bno085_soft_reset);
    } else {
        char *str = "on to trun on or off to turn off\n";
        serial_port.write(str, strlen(str));
    }
    return 0;
}

int32_t _get_linear_accelerometer_data(int32_t argc, char **argv)
{
    if (strcmp(argv[1], "on") == 0) {
        state = true;
        queue.call(bno_linear_accelerometer_data);
        led = 1;
    } else if (strcmp(argv[1], "off") == 0) {
        led = 0;
        state = false;
        queue.call(bno085_soft_reset);
    } else {
        char *str = "on to trun on or off to turn off\n";
        serial_port.write(str, strlen(str));
    }
    return 0;
}

int32_t _get_rotation_vector_data(int32_t argc, char **argv)
{
    if (strcmp(argv[1], "on") == 0) {
        state = true;
        queue.call(bno_rotation_vector_data);
        led = 1;
    } else if (strcmp(argv[1], "off") == 0) {
        led = 0;
        state = false;
        queue.call(bno085_soft_reset);
    } else {
        char *str = "on to trun on or off to turn off\n";
        serial_port.write(str, strlen(str));
    }
    return 0;
}

int32_t _get_gyroscope_data(int32_t argc, char **argv)
{
    if (strcmp(argv[1], "on") == 0) {
        state = true;
        queue.call(bno_gyroscope_data);
        led = 1;
    } else if (strcmp(argv[1], "off") == 0) {
        led = 0;
        state = false;
        queue.call(bno085_soft_reset);
    } else {
        char *str = "on to trun on or off to turn off\n";
        serial_port.write(str, strlen(str));
    }
    return 0;
}

int32_t _get_magnetometer_data(int32_t argc, char **argv)
{
    if (strcmp(argv[1], "on") == 0) {
        state = true;
        queue.call(bno_magnetometer_data);
        led = 1;
    } else if (strcmp(argv[1], "off") == 0) {
        led = 0;
        state = false;
        queue.call(bno085_soft_reset);
    } else {
        char *str = "on to trun on or off to turn off\n";
        serial_port.write(str, strlen(str));
    }
    return 0;
}

int32_t _get_step_counter_data(int32_t argc, char **argv)
{
    if (strcmp(argv[1], "on") == 0) {
        state = true;
        queue.call(bno_step_counter_data);
        led = 1;
    } else if (strcmp(argv[1], "off") == 0) {
        led = 0;
        state = false;
        queue.call(bno085_soft_reset);
    } else {
        char *str = "on to trun on or off to turn off\n";
        serial_port.write(str, strlen(str));
    }
    return 0;
}

int32_t _get_calibration_data(int32_t argc, char **argv)
{
    if (strcmp(argv[1], "on") == 0) {
        state = true;
        queue.call(bno_calibration_data);
        led = 1;
    } else if (strcmp(argv[1], "off") == 0) {
        led = 0;
        state = false;
        queue.call(bno085_soft_reset);
    } else {
        char *str = "on to trun on or off to turn off\n";
        serial_port.write(str, strlen(str));
    }
    return 0;
}

int32_t _get_activity_classifier_data(int32_t argc, char **argv)
{
    if (strcmp(argv[1], "on") == 0) {
        state = true;
        queue.call(bno_activity_classifier_data);
        led = 1;
    } else if (strcmp(argv[1], "off") == 0) {
        led = 0;
        state = false;
        queue.call(bno085_soft_reset);
    } else {
        char *str = "on to trun on or off to turn off\n";
        serial_port.write(str, strlen(str));
    }
    return 0;
}

int32_t _get_fast_gyroscope_data(int32_t argc, char **argv)
{
    if (strcmp(argv[1], "on") == 0) {
        state = true;
        queue.call(bno_fast_gyroscope_data);
        led = 1;
    } else if (strcmp(argv[1], "off") == 0) {
        led = 0;
        state = false;
        queue.call(bno085_soft_reset);
    } else {
        char *str = "on to trun on or off to turn off\n";
        serial_port.write(str, strlen(str));
    }
    return 0;
}

int32_t _get_gravity_data(int32_t argc, char **argv)
{
    if (strcmp(argv[1], "on") == 0) {
        state = true;
        queue.call(bno_gravity_data);
        led = 1;
    } else if (strcmp(argv[1], "off") == 0) {
        led = 0;
        state = false;
        queue.call(bno085_soft_reset);
    } else {
        char *str = "on to trun on or off to turn off\n";
        serial_port.write(str, strlen(str));
    }
    return 0;
}

int32_t _get_angles_y_p_r_data(int32_t argc, char **argv)
{
    if (strcmp(argv[1], "on") == 0) {
        state = true;
        queue.call(bno_angles_y_p_r_data);
        led = 1;
    } else if (strcmp(argv[1], "off") == 0) {
        led = 0;
        state = false;
        queue.call(bno085_soft_reset);
    } else {
        char *str = "on to trun on or off to turn off\n";
        serial_port.write(str, strlen(str));
    }
    return 0;
}

int32_t _get_timestamp_data(int32_t argc, char **argv)
{
    if (strcmp(argv[1], "on") == 0) {
        state = true;
        queue.call(bno_timestamp_data);
        led = 1;
    } else if (strcmp(argv[1], "off") == 0) {
        led = 0;
        state = false;
        queue.call(bno085_soft_reset);
    } else {
        char *str = "on to trun on or off to turn off\n";
        serial_port.write(str, strlen(str));
    }
    return 0;
}

int32_t _get_tare_rotation_vector_data(int32_t argc, char **argv)
{
    if (strcmp(argv[1], "on") == 0) {
        state = true;
        queue.call(bno_tare_rotation_vector_data);
        led = 1;
    } else if (strcmp(argv[1], "off") == 0) {
        led = 0;
        state = false;
        queue.call(bno085_soft_reset);
    } else {
        char *str = "on to trun on or off to turn off\n";
        serial_port.write(str, strlen(str));
    }
    return 0;
}

/**
 * \brief           Application output function
 * \param[in]       str: String to print, null-terminated
 * \param[in]       lw: LwSHELL instance
 */
void shell_output(const char *str, lwshell_t *lw)
{
    serial_port.write(str, strlen(str));
    if (*str == '\r') {
        serial_port.write("\n", 1);
    }
}

void on_rx_interrupt()
{
    char c;

    if (serial_port.read(&c, 1)) {
        lwshell_input(&c, 1);
    }
}

int main(void)
{

    serial_port.baud(9600);
    serial_port.format(
            /* bits */ 8,
            /* parity */ SerialBase::None,
            /* stop bit */ 1);

    /* Init library */
    lwshell_init();

    /* Add optional output function for the purpose of the feedback */
    lwshell_set_output_fn(shell_output);

    /* Define shell commands */
    lwshell_register_cmd("led", _do_led, "This command allow too control LED");
    lwshell_register_cmd(
            "accelerometer", _get_accelerometer_data, "This command enable feature accelerometer");
    lwshell_register_cmd("linear_accelerometer",
            _get_linear_accelerometer_data,
            "This command enable feature linear accelerometer");
    lwshell_register_cmd("rotation_vector",
            _get_rotation_vector_data,
            "This command enable feature rotation vector");
    lwshell_register_cmd("gyroscope", _get_gyroscope_data, "This command enable feature gyroscope");
    lwshell_register_cmd(
            "magnetometer", _get_magnetometer_data, "This command enable feature magnetometer");
    lwshell_register_cmd(
            "step_counter", _get_step_counter_data, "This command enable feature step counter");
    lwshell_register_cmd("calibration_classifier",
            _get_calibration_data,
            "This command enable feature calibration classifier");
    lwshell_register_cmd("activity_classifier",
            _get_activity_classifier_data,
            "This command enable feature Activity classifier");
    lwshell_register_cmd("fast_gyroscope",
            _get_fast_gyroscope_data,
            "This command enable feature fast gyroscope");
    lwshell_register_cmd("gravity", _get_gravity_data, "This command enable feature gravity");
    lwshell_register_cmd("angles_y_p_r",
            _get_angles_y_p_r_data,
            "This command enable feature angles Yaw Pitch Roll");
    lwshell_register_cmd("timestamp",
            _get_timestamp_data,
            "This command enable feature angles Yaw Pitch Roll + Timestamp in ms");
    lwshell_register_cmd("tare_rotation_vector",
            _get_tare_rotation_vector_data,
            "This command enable feature Tare rotation vector + print it");

    /* User input to process every character */
    printf("Start entering your command and press enter...\r\n");
    bno085.initialize();
    printf("\n\n\n\nBNO085 test application, choose a demo :\n");
    printf("1. accelerometer\n");
    printf("2. linear_accelerometer\n");
    printf("3. gyroscope\n");
    printf("4. rotation Vector\n");
    printf("5. magnetometer\n");
    printf("6. step_counter\n");
    printf("7. calibration\n");
    printf("8. activity_classifier\n");
    printf("9. fast_gyroscope\n");
    printf("10. gravity\n");
    printf("11. angles_y_p_r\n");
    printf("12. timestamp\n");
    printf("13. tare_rotation_vector\n");

    serial_port.attach(&on_rx_interrupt, SerialBase::RxIrq);
    queue.dispatch_forever();

    while (1) {
        ThisThread::sleep_for(1s);
    }
    return 0;
}
