esp-serial-terminal
==========================
ESP8266 WiFi - RS232 мост. Дополнительная функция - управление реле, применимо для замыкания контактов кнопки питания ПК.
Разработано для реализации удаленного терминала для домашнего сервера. Позволяет работать с системой до загрузки сетевых интерфейсов и возможности использования SSH/VNC. 
Например, управление grub, ввод пароля от зашифрованного корневого раздела, решение проблем отказа монтирования ФС, или просто ttyS консоль.

Прошивка форкнута от [ESP8266-transparent-bridge project](https://github.com/beckdac/ESP8266-transparent-bridge).
Использовалась ESP8266 KiCAD библиотека [techinc-kicad-lib](https://github.com/techinc/techinc-kicad-lib).

Собранная прошивка в [релизах] (https://github.com/dima-ch/esp-serial-terminal/releases).

#Аппаратная часть
На текущий момент аппаратная часть доступна для модуля ESP8266 ESP-03. Схема и печатная плата в формате KiCAD лежат в каталоге schematic. GPIO2 модуля подключен к светодиоду индикации.
GPIO14 подключен к реле. Выбран GPIO14 поскльку логический уровень на нем не прыгает при включение ESP8266.

#Индикация
* Светодиод моргает  во время работы
* Когда подключен клиент светодиод моргает медленнее

Подключение удобно производить клиентами telnet. 
ConnectBot для Android работает с настройками по умолчанию, утилита Linux telnet требует переменной окружения export TERM=VT100 и выполнения после подключения ^]mode character.

#Конфигурация
Следует убедиться, что в коде установлен #define CONFIG_DYNAMIC

Конфигурационные и управляющие AT команды
```
+++AT                                    # вывод в ответ OK
+++AT PWBTN <duration: SHORT, LONG, HARDRESET>
										 # Управление контактами реле: SHORT - замыкание на 0.5с, LONG - на 4с, HARDRESET - 4c, пауза в 1c, затем 0.5с
+++AT MODE                               # вывод текущего режима
+++AT MODE <mode: 1= STA, 2= AP, 3=both> # установка режима
+++AT STA                                # вывести текущий ssid и пароль подключения к точке доступа
+++AT STA <ssid> <password>              # установить ssid и пароль к точке доступа
+++AT AP                                 # вывести текущие настройки точки доступа
+++AT AP <ssid>                          # установить режим открытой точки доступа с указанным ssid
+++AT AP <ssid> <pw> [<authmode> [hide-ssid [<ch>]]]]
										 # установить параметры точки доступа, authmode:1= WEP,2= WPA,3= WPA2,4= WPA+WPA2, 
										 # hide-ssid:1-hide, 0-show(not hide), channel: 1..13
+++AT BAUD                               # вывести текущие настройки UART
+++AT BAUD <baud> [data [parity [stop]]] # установить битрейт и опционально data bits = 5/6/7/8 , parity = N/E/O, stop bits = 1/1.5/2
+++AT PORT                               # вывести текущей TCP порт
+++AT PORT <port>                        # установить TCP порт (перезагрузка)
+++AT FLASH                              # вывод настроек сохранения
+++AT FLASH <1|0>                        # 1: изменение настроек UART (++AT BAUD ...) сохраняется (по умолчанию), 0: изменения не сохраняются
+++AT RESET                              # программная перезагрузка
```
В случае успеха все команды возвращают "OK" или соответствующий вывод. На заметку, пароль и ssid не могут содержать пробелов.

Пример конфигурирования. telnet используется без ключей и настроек в отличие от режима работы. Если нет ответа на команду +++AT при ручном вводе, рекомендую вставлять команду в терминал из буфера обмена.
```
user@host:~$ telnet 192.168.1.197
Trying 192.168.1.197...
Connected to 192.168.1.197.
Escape character is '^]'.
+++AT MODE
MODE=3
OK
+++AT AP
SSID=ESP_9E2EA6 PASSWORD= AUTHMODE=0 IS_HIDDEH_SSID=0 CHANNEL=3
OK
+++AT AP newSSID password
OK
+++AT AP
SSID=newSSID PASSWORD=password AUTHMODE=2 IS_HIDDEH_SSID=0 CHANNEL=3
OK
+++AT AP ESP_9E2EA6
OK
+++AT AP
SSID=ESP_9E2EA6 PASSWORD= AUTHMODE=0 CHANNEL=3
OK
^]c

telnet> c
Connection closed.
```

Прошивка:
```
esptool.py --port /dev/ttyXXX write_flash 0x00000 0x00000.bin 0x40000 0x40000.bin
...
или с использованием ESP8266Flasher.exe из https://github.com/nodemcu/nodemcu-flasher с
0x00000.bin по адресу 0x00000
0x40000.bin по адресу 0x40000

Дополнительно см. [ESP8266-transparent-bridge project](https://github.com/beckdac/ESP8266-transparent-bridge)

#TODO
* ошибка на печатной плате и схеме, перепутаны пины DB9
* уменьшение клиентов до 2 и увеличение буфера отправки
