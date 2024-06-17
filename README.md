# Mbed Ceva BNO085 example

Mbed OS example for 9 axis CEVA BNO085.

# Usage

This application demonstrates the capabilities of the BNO085 motion sensor.
Connect a virtual terminal to the Zest_Core board and follow the instructions to select the demo.

```
mbed sterm -p /dev/ttyUSB0 -b 9600
```

When you see the menu of this test application you are done (if you don't see it try to press **RESET** button) :

```
BNO085 test application, choose a demo :
1. accelerometer
2. linear_accelerometer
3. gyroscope
4. rotation Vector
5. magnetometer
6. step_counter
7. calibration
8. activity_classifier
9. fast_gyroscope
10. gravity
11. angles_y_p_r
12. timestamp
13. tare_rotation_vector
```

To try a demo, turn it on by using its name indicated in the menu:

```shell
# Turn accelerometer demo on
accelerometer on
```

To switch the demo, turn off the first demo, then turn on the new demo:

```shell
# Turn accelerometer demo off
accelerometer off
# Turn gravity demo on
gravity on
```

You can also reset the Zest_Core board to restart the application and start a new demo.

## Requirements

### Hardware requirements

The following boards are required:

- Zest_Core_STM32L4A6RG
- Zest_Sensor_IMU, V3.0.0, with mounted BNO085

### Software requirements

This application makes use of the following libraries (automatically
imported by `mbed deploy` or `mbed import`):

- [LwSHELL](https://github.com/MaJerle/lwshell)
- [Mbed OS](https://github.com/ARMmbed/mbed-os)
- [BNO085](https://github.com/catie-aq/mbed_ceva-bno085)

## How to compile

To clone **and** deploy the project in one command, use `mbed import` and skip to the
target enabling instructions:

```shell
mbed import https://github.com/catie-aq/mbed_ceva-bno085-example.git
```

Alternatively:

- Clone to "mbed_ceva-bno085-example" and enter it:

  ```shell
  git clone https://github.com/catie-aq/mbed_ceva-bno085-example.git
  cd mbed_ceva-bno085-example
  ```

- Deploy software requirements with:
  ```shell
  mbed deploy
  ```

Enable the custom [target](https://github.com/catie-aq/mbed_zest-core-stm32l4a6rg):

```shell
cp zest-core-stm32l4a6rg/custom_targets.json .
```

Compile the project:

```shell
mbed compile
```

Program the target device with a Segger J-Link debug probe and
[`sixtron_flash`](https://github.com/catie-aq/6tron_flash) tool:

```shell
sixtron_flash stm32l4a6rg BUILD/ZEST_CORE_STM32L4A6RG/GCC_ARM/mbed_ceva-bno085-example.elf
```

Debug on the target device with the probe and Segger
[Ozone](https://www.segger.com/products/development-tools/ozone-j-link-debugger)
software.
