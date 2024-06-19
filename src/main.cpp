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
#define SENSOR_REPORTID_PRY                                                                        \
    50 // Create ID REPORT to handle Demo of Quaternion and Pitch Roll Yaw (they have the same
       // sensor REPORT ID)
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

void display_values(
        int sensor_id, float x, float y, float z, float real, string timestamp, string accuracy)
{
    string new_real = to_string(real);
    switch (sensor_id) {
        case SENSOR_REPORTID_ACCELEROMETER: {
            printf("[%s]\t\taccel x : %f\t\t\taccel y : %f\t\t\taccel z : %f\t\t\t[%s]\n",
                    timestamp,
                    x,
                    y,
                    z,
                    accuracy);
            break;
        }
        case SENSOR_REPORTID_ROTATION_VECTOR: {
            printf("[%s] \t Rotation Quat x : %f\tRotation Quat x : %f\tRotation Quat x : "
                   "%f\tRotation Quat x : %f\t[%s]",
                    timestamp,
                    x,
                    y,
                    z,
                    real,
                    accuracy);
            break;
        }
        case SENSOR_REPORTID_LINEAR_ACCELERATION: {
            printf("[%s]\t\taccel x : %f\t\t\taccel y : %f\t\t\taccel z : %f\t\t\t[%s]\n",
                    timestamp,
                    x,
                    y,
                    z,
                    accuracy);
            break;
        }
        case SENSOR_REPORTID_GYROSCOPE: {
            printf("[%s]\t\tgyro x : %f\t\t\tgyro y : %f\t\t\tgyro z : %f\t\t\t[%s]\n",
                    timestamp,
                    x,
                    y,
                    z,
                    accuracy);
            break;
        }
        case SENSOR_REPORTID_GYRO_INTEGRATED_ROTATION_VECTOR: {
            printf("[%s]\t\tFgyro x : %f\t\t\tFgyro y : %f\t\t\tFgyro z : %f\t\t\t[%s]\n",
                    timestamp,
                    x,
                    y,
                    z,
                    accuracy);
            break;
        }
        case SENSOR_REPORTID_MAGNETIC_FIELD: {
            printf("[%s]\t\tMag x : %f\t\t\tMag y : %f\t\t\tMag z : %f\t\t\t[%s]\n",
                    timestamp,
                    x,
                    y,
                    z,
                    accuracy);
            break;
        }
        case SENSOR_REPORTID_GRAVITY: {
            printf("[%s]\t\tGravity x : %f\t\t\tGravity y : %f\t\t\tGravity z : %f\t\t\t[%s]\n",
                    timestamp,
                    x,
                    y,
                    z,
                    accuracy);
            break;
        }
        case SENSOR_IDREPORT_PRY: {
            printf("[%s]\t\tpitch : %f\t\t\tRoll : %f\t\t\tYaw z : %f\t\t\t[%s]\n",
                    timestamp,
                    x,
                    y,
                    z,
                    accuracy);
            break;
        }
    }
}

void bno_accelerometer_data()
{
    bno085.enable_accelerometer(5);
    while (state) {
        if (bno085.get_available_data()) {
            bno085.get_readings();
            uint8_t IdReport = bno085.get_readings();
            string timestamp = to_string(bno085.get_time_stamp());
            string accuracy = to_string(bno085.get_accel_accuracy());
            display_values(IdReport,
                    bno085.get_accel_x(),
                    bno085.get_accel_y(),
                    bno085.get_accel_z(),
                    0.000,
                    timestamp,
                    accuracy);
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
            uint8_t IdReport = bno085.get_readings();
            string timestamp = to_string(bno085.get_time_stamp());
            string accuracy = to_string(bno085.get_accel_accuracy());
            display_values(IdReport,
                    bno085.get_accel_x(),
                    bno085.get_accel_y(),
                    bno085.get_accel_z(),
                    0.000,
                    timestamp,
                    accuracy);
        }
    }
}

void bno_rotation_vector_data()
{
    bno085.enable_rotation_vector(50);
    while (state) {
        if (bno085.get_available_data()) {
            uint8_t ID_report = bno085.get_readings();
            display_values(ID_report,
                    bno085.get_quat_i(),
                    bno085.get_quat_j(),
                    bno085.get_quat_k(),
                    bno085.get_quat_real(),
                    to_string(bno085.get_time_stamp()),
                    to_string(bno085.get_quat_radian_accuracy()));
        }
    }
}

void bno_gyroscope_data()
{
    bno085.enable_gyro(50);
    while (state) {
        if (bno085.get_available_data()) {
            uint8_t IdReport = bno085.get_readings();
            string timestamp = to_string(bno085.get_time_stamp());
            string accuracy = to_string(bno085.get_gyro_accuracy());
            display_values(IdReport,
                    bno085.get_gyro_x(),
                    bno085.get_gyro_y(),
                    bno085.get_gyro_y(),
                    0.000,
                    timestamp,
                    accuracy);
        }
    }
}

void bno_magnetometer_data()
{
    bno085.enable_magnetometer(50);
    while (state) {
        if (bno085.get_available_data()) {
            uint8_t IdReport = bno085.get_readings();
            string timestamp = to_string(bno085.get_time_stamp());
            string accuracy = to_string(bno085.get_mag_accuracy());
            display_values(IdReport,
                    bno085.get_mag_x(),
                    bno085.get_mag_y(),
                    bno085.get_mag_z(),
                    0.000,
                    timestamp,
                    accuracy);
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
            printf("Steps : %u", steps);
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
                case 0: {
                    printf("Unknown classification\n");
                    break;
                }
                case 1: {
                    printf("On table\n");
                    break;
                } break;
                case 2: {
                    printf("Stationary\n");
                    break;
                } break;
                case 3: {
                    printf("Stable\n");
                    break;
                } break;
                case 4: {
                    printf("Motion\n");
                    break;
                } break;
                case 5: {
                    printf("Reserved\n");
                    break;
                } break;
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
    string activity;
    bno085.enable_activity_classifier(500, enable_activities, activity_confidences);
    while (state) {
        if (bno085.get_available_data()) {
            if (bno085.get_readings()) {
                uint8_t activity_number = bno085.get_activity_classifier();
                if (activity_number == 0)
                    activity = "Unknown\n";
                else if (activity_number == 1)
                    activity = "In vehicle\n";
                else if (activity_number == 2)
                    activity = "On bicycle\n";
                else if (activity_number == 3)
                    activity = "On foot\n";
                else if (activity_number == 4)
                    activity = "Still\n";
                else if (activity_number == 5)
                    activity = "Tilting\n";
                else if (activity_number == 6)
                    activity = "Walking\n";
                else if (activity_number == 7)
                    activity = "Running\n";
                else if (activity_number == 8)
                    activity = "On stairs\n";
                printf("%s", activity);
            }
        }
    }
}

void bno_fast_gyroscope_data()
{
    bno085.enable_gyro_integrated_rotation_vector(50);
    while (state) {
        if (bno085.get_available_data()) {
            uint8_t IdReport = bno085.get_readings();
            display_values(IdReport,
                    bno085.get_fast_gyro_x(),
                    bno085.get_fast_gyro_y(),
                    bno085.get_fast_gyro_z(),
                    0.000,
                    "N/A",
                    "N/A");
        }
    }
}

void bno_gravity_data()
{
    bno085.enable_gravity(50);
    while (state) {
        if (bno085.get_available_data()) {
            uint8_t IdReport = bno085.get_readings();
            string timestamp = to_string(bno085.get_time_stamp());
            string accuracy = to_string(bno085.get_gravity_accuracy());
            display_values(IdReport,
                    bno085.get_gravity_x(),
                    bno085.get_gravity_y(),
                    bno085.get_gravity_z(),
                    0.000,
                    timestamp,
                    accuracy);
        }
    }
}

void bno_angles_y_p_r_data()
{
    bno085.enable_rotation_vector(50);
    while (state) {
        if (bno085.get_available_data()) {
            uint8_t IdReport = bno085.get_readings();
            float pitch = bno085.get_pitch() * 180 / pi;
            float roll = bno085.get_roll() * 180 / pi;
            float yaw = bno085.get_yaw() * 180 / pi;
            string timestamp = to_string(bno085.get_time_stamp());
            string accuracy = to_string(bno085.get_quat_radian_accuracy());
            display_values(SENSOR_IDREPORT_PRY, pitch, roll, yaw, 0.000, timestamp, accuracy);
        }
    }
}

void bno_timestamp_data()
{
    bno085.enable_rotation_vector(50);
    while (state) {
        if (bno085.get_available_data()) {
            uint8_t IdReport = bno085.get_readings();
            float pitch = bno085.get_pitch() * 180 / pi;
            float roll = bno085.get_roll() * 180 / pi;
            float yaw = bno085.get_yaw() * 180 / pi;
            string timestamp = to_string(bno085.get_time_stamp());
            string accuracy = to_string(bno085.get_quat_radian_accuracy());
            display_values(SENSOR_IDREPORT_PRY, pitch, roll, yaw, 0.000, timestamp, accuracy);
        }
    }
}

void bno_tare_rotation_vector_data()
{
    string tarestr;
    bno085.enable_rotation_vector(50);
    while (state) {
        if (bno085.get_available_data()) {
            tare_counter++; // each 50ms increments tare_counter
            uint8_t IdReport = bno085.get_readings();
            float QuatI = bno085.get_quat_i() * 180 / pi;
            float QuatJ = bno085.get_quat_j() * 180 / pi;
            float QuatK = bno085.get_quat_k() * 180 / pi;
            float QuatReal = bno085.get_quat_real();
            string accuracy = to_string(bno085.get_quat_radian_accuracy());

            string timestamp = to_string(bno085.get_time_stamp());
            display_values(SENSOR_IDREPORT_PRY, QuatI, QuatJ, QuatK, QuatReal, timestamp, accuracy);
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

void bno_init()
{
    bno085.initialize();
}

void print_demo_menu()
{
    state = false;
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
