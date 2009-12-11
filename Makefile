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

depend:
	makedepend -I/usr/lib/avr/include -- $(CFLAGS) -- $(SOURCES)

# DO NOT DELETE

bsp.o: proxlamp_r01_bsp.h /usr/lib/avr/include/avr/interrupt.h
bsp.o: /usr/lib/avr/include/avr/io.h /usr/lib/avr/include/avr/sfr_defs.h
bsp.o: /usr/lib/avr/include/inttypes.h /usr/lib/avr/include/stdint.h
bsp.o: /usr/lib/avr/include/avr/portpins.h /usr/lib/avr/include/avr/common.h
bsp.o: /usr/lib/avr/include/avr/version.h /usr/lib/avr/include/avr/fuse.h
bsp.o: /usr/lib/avr/include/avr/lock.h
dimmer.o: bsp.h proxlamp_r01_bsp.h dimmer.h
dimmer.o: /usr/lib/avr/include/avr/interrupt.h /usr/lib/avr/include/avr/io.h
dimmer.o: /usr/lib/avr/include/avr/sfr_defs.h /usr/lib/avr/include/inttypes.h
dimmer.o: /usr/lib/avr/include/stdint.h /usr/lib/avr/include/avr/portpins.h
dimmer.o: /usr/lib/avr/include/avr/common.h
dimmer.o: /usr/lib/avr/include/avr/version.h /usr/lib/avr/include/avr/fuse.h
dimmer.o: /usr/lib/avr/include/avr/lock.h
main.o: bsp.h proxlamp_r01_bsp.h /usr/lib/avr/include/util/delay.h
main.o: /usr/lib/avr/include/inttypes.h /usr/lib/avr/include/stdint.h
main.o: /usr/lib/avr/include/util/delay_basic.h dimmer.h ultrasonic.h sched.h
ultrasonic.o: bsp.h proxlamp_r01_bsp.h /usr/lib/avr/include/avr/interrupt.h
ultrasonic.o: /usr/lib/avr/include/avr/io.h
ultrasonic.o: /usr/lib/avr/include/avr/sfr_defs.h
ultrasonic.o: /usr/lib/avr/include/inttypes.h /usr/lib/avr/include/stdint.h
ultrasonic.o: /usr/lib/avr/include/avr/portpins.h
ultrasonic.o: /usr/lib/avr/include/avr/common.h
ultrasonic.o: /usr/lib/avr/include/avr/version.h
ultrasonic.o: /usr/lib/avr/include/avr/fuse.h /usr/lib/avr/include/avr/lock.h
ultrasonic.o: ultrasonic.h sched.h
