TARGET=proxlamp
SOURCES=$(wildcard *.c)
OBJECTS=$(SOURCES:.c=.o)

MMCU=atmega88
PROGRAMMER=dragon_isp
CC=/usr/bin/avr-gcc
CFLAGS=-g -Os -Wall -Werror -mcall-prologues -mmcu=$(MMCU) -lm
OBJ2HEX=/usr/bin/avr-objcopy 
AVRPROG=/usr/local/bin/avrdude
AVROPTS=-B5 -Pusb

$(TARGET).hex:

%.hex : %.obj
	$(OBJ2HEX) -R .eeprom -O ihex $< $@

%.obj : $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

burn: $(TARGET).hex
	$(AVRPROG) -c$(PROGRAMMER) -p$(MMCU) $(AVROPTS) -Uflash:w:$(TARGET).hex

clean :
	rm -f *.hex *.obj *.o
