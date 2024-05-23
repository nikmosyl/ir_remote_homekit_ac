# Xiaomi инфракраснный контроллер с интеграцией HomeKit
Прошивка для универсального ИК-пульта дистанционного управления Xiaomi chuangmi.remote.v2 MJYKQ01CM с интеграцией Apple HomeKit

[en](https://github.com/nikmosyl/ir_remote_homekit_ac/blob/main/README.md)

## Hardware

Вторая версия универсиального пульта получила странную особенность -- не способность считывать длинные RAW коды от кондиционеров.
Чтобы залить новую прошивку необходимо разобрать устройство.
Четыре винта под резинкой снизу:
<kbd>
  <img src="https://github.com/nikmosyl/ir_remote_homekit_ac/blob/main/ReadMe/Img/bot.jpeg" heigth=10>
</kbd>

Подсоединиться к контактным площадкам TX, RX, GND:
<kbd>
  <img src="https://github.com/nikmosyl/ir_remote_homekit_ac/blob/main/ReadMe/Img/top.jpeg" heigth=10>
</kbd>

Для прошивки нужно подать питание при нажатой кнопке RESET

## Firmware

Основано на [SDK](https://github.com/espressif/esp-homekit-sdk) от Espressif 

Общие шаги по установке c использованием исходников:

### 1. Установить [ESP-IDF](https://docs.espressif.com/projects/esp-idf/en/stable/esp32c3/get-started/linux-macos-setup.html)
Предпочительная версия v5.2.1

Предпочтительное место установки в корень домашней директории ~/


### 2. Установить [esp-homekit-sdk](https://github.com/espressif/esp-homekit-sdk)
Предпочтительное место установки в корень домашней директории ~/


### 3. Скачать репозиторий с проектом
```bash
cd ~/
git clone https://github.com/nikmosyl/ir_remote_homekit_ac.git
```

### 4. Настроить параметры HomeKit устройства и светодиодной ленты
Файл ir_remote_homekit_ac/main/app_main.c строка 147:
```C
hap_acc_cfg_t cfg = {
        .name = "Esp-AC",                  //отображаемое в приложение Дом имя устройства
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

Стандартный интерфейс HomeKit определяет 4 режима работы (Off, Cold, Hot и Auto) и температурный режим от 10 до 38 градусов цельсия
Для всех этих вариаций в файле ir_remote_homekit_ac/main/Libs/ac_command/commands_list.h предсавлены массивы для заполнения RAW кодами

Вся подготовка данных для отправки происходит в файле ir_remote_homekit_ac/main/Libs/ac_command/ac_command.c
```C
void set_mode(int mode)                    //устанавливает режим работы (выбор массива из которого брать данные)
```

```C
void set_temperature(float temperature)    //устанавливает сдвиг температуры (выбор индекса в ранее выбранном массиве)
```

### 5. Прописать переменные окружения (можно пропустить при установке idf и sdk по предпочтительным путям)
Можно либо прописать временные переменные окружения (указать путь до esp-homekit-sdk):
```bash
export HOMEKIT_PATH=~/esp/esp-homekit-sdk 
```
Либо отредоктировать файл frimware/lightbulb/CMakeLists.txt строка 11:
```CMake
set(HOMEKIT_PATH ~/esp/esp-homekit-sdk)
```
вместо ~/esp/esp-homekit-sdk указать свой путь к каталогу esp-homekit-sdk

### 6. Установить пароль Wi-Fi
Можно либо оставить всё как есть и тогда при первом запуске в консоль выведется QR код для подключения через [Espressif Provisioning Apps](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/provisioning/provisioning.html#provisioning-tools) 

Либо зашить пароль от Wi-Fi в контроллер жёстко:
```bash
cd ~/ir_remote_homekit_ac
. ~/esp/esp-idf/export.sh
idf.py menuconfig
```

В разделе _**App Wi-Fi** -> **Source of Wi-Fi Credentials**_ нужно установить галочку на **Use Hardcoded**

В появившиеся поля необходимо ввести имя и пароль точки доступа

Нажать S (Save) и Enter после этого

### 7. Устранение проблем с CRC
```bash
cd ~/ir_remote_homekit_ac
. ~/esp/esp-idf/export.sh
idf.py menuconfig
```
В разделе _**Component config** -> **Hardware Settings** -> **MAC Config**_ нужно установить галочку на **Ignore MAC CRC**

### 8. Прошить плату

Перевести контроллер в режим прошивки (подать питание при нажатой кнопке RESET)

Собрать прошивку:
```bash
idf.py build
```
Загрузить прошивку:
```bash
idf.py flash
```

### 9. После прошивки
Перезагрузить плату
Дождаться пока она подключится к Wi-Fi и отобразится в меню добавления новых устройств HomeKit (на iPhone приложение _**Дом** -> **+** -> **Другие параметры** -> **выберете устройство**_, код 11122333) 





