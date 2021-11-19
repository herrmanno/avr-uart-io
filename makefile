# COPIED FROM https://de.wikibooks.org/wiki/C-Programmierung_mit_AVR-GCC/_Makefile

TARGET=uart_io
SOURCES=uart_io.c
HEADER_PATH=/usr/local/include/
LIBRARY_PATH=/usr/local/lib/

MCU=atmega328p
F_CPU=16000000L # 16MHz = Max CPU speed (https://en.wikipedia.org/wiki/ATmega328)
BAUD=9600

OBJECTS=$(SOURCES:.c=.o)
CFLAGS=-c -Os -shared -DF_CPU=$(F_CPU) -DBAUD=$(BAUD)
# Link against (non-avr) printf library to support floating formatting
# see https://stackoverflow.com/a/14146944/2720791
LDFLAGS=-Wl,-u,vfprintf -lprintf_flt

all: library

install:
	cp *h $(HEADER_PATH)
	cp *a $(LIBRARY_PATH)

library: $(OBJECTS)
	avr-ar -rcs lib$(TARGET).a $(OBJECTS)
	# avr-objdump -h -S lib$(TARGET).a > lib$(TARGET).lss

size:
	avr-size --format=avr --mcu=atmega328p lib$(TARGET).a

objects: $(OBJECTS)

.c.o:
	avr-gcc $(LDFLAGS) $(CFLAGS) -mmcu=$(MCU) $< -o $@

clean:
	rm -rf *.o
	rm -rf *.a
