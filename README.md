# Xiaomi universal infrared remote control with HomeKit integration
Firmware for Xiaomi universal IR remote control chuangmi.remote.v2 MJYKQ01CM with Apple HomeKit integration

[ru](https://github.com/nikmosyl/ir_remote_homekit_ac/blob/main/ReadMe/README-ru.md)

## Hardware

The second version of the universal remote control received a strange feature - it is not able to read long RAW codes from air conditioners.
To upload new firmware, you need to disassemble the device.
Four screws under the rubber band at the bottom:
<kbd>
  <img src="https://github.com/nikmosyl/ir_remote_homekit_ac/blob/main/ReadMe/Img/bot.jpeg" heigth=10>
</kbd>

Connect to TX, RX, GND pads:

<kbd>
  <img src="https://github.com/nikmosyl/ir_remote_homekit_ac/blob/main/ReadMe/Img/top.jpeg" heigth=10>
</kbd>

To flash the firmware, you need to apply power while the RESET button is pressed.

## Firmware

Based on [SDK](https://github.com/espressif/esp-homekit-sdk) by Espressif 

General installation steps using sources:

### 1. Install [ESP-IDF](https://docs.espressif.com/projects/esp-idf/en/stable/esp32c3/get-started/linux-macos-setup.html)
Preferred version v5.2.1

Preferred installation location is at the root of your home directory ~/


### 2. Install [esp-homekit-sdk](https://github.com/espressif/esp-homekit-sdk)
Preferred installation location is at the root of your home directory ~/


### 3. Download the repository with the project
```bash
cd ~/
git clone https://github.com/nikmosyl/ir_remote_homekit_ac.git
```

### 4. Configure HomeKit device and LED strip settings
File ir_remote_homekit_ac/main/app_main.c line 147:
```C
hap_acc_cfg_t cfg = {
        .name = "Esp-AC",                  //device name displayed in the Home application
        .manufacturer = "Espressif",
        .model = "EspAir01",
        .serial_num = "202405211633",
        .fw_rev = "1.0.0",
        .hw_rev = "1.0",
        .pv = "1.1.0",
        .identify_routine = thermostat_identify,
        .cid = HAP_CID_THERMOSTAT,
};
```

The standard HomeKit interface defines 4 operating modes (Off, Cold, Hot and Auto) and a temperature range from 10 to 38 degrees Celsius
For all these variations, the file ir_remote_homekit_ac/main/Libs/ac_command/commands_list.h contains arrays for filling with RAW codes

All data preparation for sending occurs in the file ir_remote_homekit_ac/main/Libs/ac_command/ac_command.c
```C
void set_mode(int mode)                    //sets the operating mode (selecting an array from which to take data)
```

```C
void set_temperature(float temperature)    //sets the temperature shift (selecting an index in a previously selected array)
```

### 5. Register environment variables (can be skipped when installing idf and sdk in preferred paths)
You can either register temporary environment variables (specify the path to esp-homekit-sdk):
```bash
export HOMEKIT_PATH=~/esp/esp-homekit-sdk 
```
Or edit the file firmware/lightbulb/CMakeLists.txt line 11:
```CMake
set(HOMEKIT_PATH ~/esp/esp-homekit-sdk)
```
Instead of ~/esp/esp-homekit-sdk, specify your path to the esp-homekit-sdk directory

### 6. Set Wi-Fi password
You can either leave everything as is, and then the first time you launch it, a QR code will be displayed in the console for connecting via [Espressif Provisioning Apps](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/provisioning/provisioning.html#provisioning-tools) 

Or hardcode the Wi-Fi password into the controller:
```bash
cd ~/ir_remote_homekit_ac
. ~/esp/esp-idf/export.sh
idf.py menuconfig
```

In the section _**App Wi-Fi** -> **Source of Wi-Fi Credentials**_ you need to check the box for **Use Hardcoded**

In the fields that appear, you must enter the name and password of the access point

Press S (Save) and Enter after that

### 7. Troubleshooting CRC problems
```bash
cd ~/ir_remote_homekit_ac
. ~/esp/esp-idf/export.sh
idf.py menuconfig
```
In the section _**Component config** -> **Hardware Settings** -> **MAC Config**_ you need to check the box for _**Ignore MAC CRC**_

### 8. Flash the board

Switch the controller to firmware mode (apply power while the RESET button is pressed)

Build firmware:
```bash
idf.py build
```
Flash firmware:
```bash
idf.py flash
```

### 9. After the firmware
Reboot the board
Wait until it connects to Wi-Fi and appears in the menu for adding new HomeKit devices (on iPhone, _**Home** app -> **+** -> **More options** -> **select device**_ , code 11122333)
