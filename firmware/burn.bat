@echo off


::set ac=C:\WinAVR-20100110
:: 2020-09-07, download avr-gcc 3.6 from Microchip
set ac=C:\avr8-gnu-toolchain-win32_x86

path %ac%\bin;%ac%\utils\bin;%path%;

avr-gcc.exe -dumpversion

set mcu=atmega328p

set main=DTMF_Decoder.ino
set hex=DTMF_Decoder.boot.hex
::set hex=DTMF_Decoder.hex
::set COM=-PCOM9
::set baud=-b115200

:: Arduino Nano default fuse setting, ATmega328p, 16MHZ Xtal
set lock=0x0f
set lfuse=0xff
set hfuse=0xda
set efuse=0xfd




goto burn_hex





avr-gcc.exe -xc -Os -mmcu=%mcu% -Wall -g -o %main%.out *.ino

::avr-gcc.exe -O2 -Wl,-Map,%1.map -o %1.out %1.c %2 %3 -mmcu=at%mcu%
cmd /c avr-objdump.exe -h -S %main%.out >%main%.lst
cmd /c avr-objcopy.exe -O ihex %main%.out %main%.hex
avr-size.exe %main%.out
del %main%.out

pause
::goto end
::pboot.exe -c1 -b19200 -p%main%.hex
::l.exe -b9600
:end








:burn_hex:

:::: burn hex
avrdude -c usbtiny -p %mcu% -U flash:w:%hex%:a -U lfuse:w:%lfuse%:m -U hfuse:w:%hfuse%:m U efuse:w:%efuse%:m U lock:w:%lock%:m

:: burn hex with Arduino bootloader, do not change fuses setting
::avrdude -c arduino -PCOM9 -b115200 -v -p %mcu% -D -U flash:w:%hex%:a

::avrdude -c arduino %COM% %baud% -v -p %mcu% -D -U flash:w:%hex%:a


::avrdude -c usbtiny -p %mcu% -U flash:w:%hex%:a

:::: avrdude terminal only
::avrdude -c usbtiny -p %mcu% -t

pause
:end
