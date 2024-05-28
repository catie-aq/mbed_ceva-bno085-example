# mbed_ceva-bno085
Mbed OS driver for 9 axis CEVA BNO085

## Requirements
### Hardware requirements
The following boards are required:
- *List mbed_ceva-bno085 hardware requirements here*

### Software requirements
mbed_ceva-bno085 makes use of the following libraries (automatically
imported by `mbed deploy` or `mbed import`):
- *List mbed_ceva-bno085 software requirements here*

## Usage
To clone **and** deploy the project in one command, use `mbed import` and skip to the
target enabling instructions:
```shell
mbed import https://github.com/catie-aq/mbed-ceva-bno085.git mbed-ceva-bno085
```

Alternatively:

- Clone to "mbed-ceva-bno085" and enter it:
  ```shell
  git clone https://github.com/catie-aq/mbed-ceva-bno085.git mbed-ceva-bno085
  cd mbed-ceva-bno085
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
sixtron_flash stm32l4a6rg BUILD/ZEST_CORE_STM32L4A6RG/GCC_ARM/mbed-ceva-bno085.elf
```

Debug on the target device with the probe and Segger
[Ozone](https://www.segger.com/products/development-tools/ozone-j-link-debugger)
software.
