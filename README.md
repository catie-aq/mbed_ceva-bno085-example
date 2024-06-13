# Mbed Ceva BNO085 example
Mbed OS example for 9 axis CEVA BNO085

# Usage
- Demo using the BNO085 with [lwshell](https://github.com/MaJerle/lwshell) You can simply call the functionality of the BNO085 with a command. Follow the instructions below :
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
if you want to try an example of [feature](#list-of-feature_name-demo-name) in shell :
```
feature_name on
``` 
If you want to try another feature, such as gravity, put off the first feature and enable gravity (You can also press **RESET** button to restart all of the demo).

```
feature_name off
gravity on
```
## Requirements
### Hardware requirements
The following boards are required:
- Zest_Core_STM32L4A6RG
- Zest_Sensor_IMU, V3.0.0, with mounted BNO085

### Software requirements
mbed_ceva-bno085-example makes use of the following libraries (automatically
imported by `mbed deploy` or `mbed import`):
- [lwshell](https://github.com/MaJerle/lwshell)
- [mbed-os](https://github.com/ARMmbed/mbed-os)
- [bno085](https://github.com/catie-aq/mbed_ceva-bno085)

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

Enable the custom target:
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

### List of feature_name demo name
- accelerometer
- linear_accelerometer
- gyroscope
- rotation Vector
- magnetometer
- step_counter
- calibration
- activity (for activity_classifier)
- fast_gyroscope
- gravity
- angles_y_p_r
- timestamp
- tare_rotation_vector (use **PUSH** button to clear tare)