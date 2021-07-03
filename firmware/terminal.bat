@echo off

set mcu=atmega328p
set hex=DTMF_Decoder.boot.hex
set COM=-PCOM9
set baud=-b115200

:: Arduino Nano fuse setting, ATmega328p, 16MHZ Xtal
set lock=0x0f
set lfuse=0xff
set hfuse=0xda
set efuse=0xfd


::set ac=C:\WinAVR-20100110
:: 2020-09-07, download avr-gcc 3.6 from Microchip
set ac=C:\avr8-gnu-toolchain-win32_x86

path %ac%\bin;%ac%\utils\bin;%path%;

avr-gcc.exe -dumpversion

:::: burn hex
::avrdude -c usbtiny -p %mcu% -U flash:w:%hex%:a -U lfuse:w:%lfuse%:m  -U hfuse:w:%hfuse%:m

:: burn hex with Arduino bootloader, do not change fuses setting
::avrdude -c arduino -PCOM9 -b115200 -v -p %mcu% -D -U flash:w:%hex%:a

::avrdude -c arduino %COM% %baud% -v -p %mcu% -D -U flash:w:%hex%:a


::avrdude -c usbtiny -p %mcu% -U flash:w:%hex%:a

:::: avrdude terminal only
::avrdude -c usbtiny -p %mcu% -t
avrdude -c arduino %COM% %baud% -p %mcu% -t