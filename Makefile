MCU=atmega32u4
FUSE_L=0xFF
FUSE_H=0xD8
F_CPU=16000000
CC=avr-gcc
LD=avr-ld
OBJCOPY=avr-objcopy
SIZE=avr-size
AVRDUDE=avrdude
#PROGRAMMER=usbasp
PROGRAMMER=avr109
CFLAGS=-Wall -g -Os -I.
CFLAGS_AVR=-mmcu=${MCU} -DF_CPU=${F_CPU} -Ilight_ws2812
TARGET=witchstaff

SRCS = main.c
SRCS_AVR = light_ws2812/light_ws2812.c
	
all:
	${CC} ${CFLAGS} ${CFLAGS_AVR} -o ${TARGET}.o ${SRCS} ${SRCS_AVR}
	${LD} -o ${TARGET}.elf ${TARGET}.o
	${OBJCOPY} -j .text -j .data -O ihex ${TARGET}.o ${TARGET}.hex
	${SIZE} -C --mcu=${MCU} ${TARGET}.elf

unix:
	gcc ${CFLAGS} -DNIX_TEST=1 -o ${TARGET}.x86_64 ${SRCS}

flash:
	${AVRDUDE} -p ${MCU} -c ${PROGRAMMER} -U flash:w:${TARGET}.hex:i -F -P usb

fuse:
	$(AVRDUDE) -p ${MCU} -c ${PROGRAMMER} -U hfuse:w:${FUSE_H}:m -U lfuse:w:${FUSE_L}:m
	
clean:
	rm -f *.c~ *.o *.elf *.hex *.x86_64
