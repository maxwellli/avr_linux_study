PRG = demo
OBJ = demo.o

MCU_TARGET = atmega16
OPTIMIZE = -O2

DEFS =
LIBS =

CC = avr-gcc
PRGMER = avrdude
PRGDEV = usbasp

override CFLAGS = -g -Wall $(OPTIMIZE) -mmcu=$(MCU_TARGET) $(DEFS)
override LDFLAGS = -Wl,-Map,$(PRG).map

OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump

all: $(PRG).elf lst text eeprom

$(PRG).elf : $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

demo.o : demo.c iocompat.h

clean:
	rm -rf *.o $(PRG).elf *.eps *.png *.pdf *.bak
	rm -rf *.lst *.map $(EXTRA_CLEAN_FILES)

lst : $(PRG).lst

%.lst : %.elf
	$(OBJDUMP) -h -S $< > $@

text : hex bin srec

hex : $(PRG).hex
bin : $(PRG).bin
srec: $(PRG).srec

%.hex : %.elf
	$(OBJCOPY) -j .text -j .data -O ihex $< $@

%.bin : %.elf
	$(OBJCOPY) -j .text -j .data -O binary $< $@

%.srec : %.elf
	$(OBJCOPY) -j .text -j .data -O srec $< $@

EXTRA_CLEAN_FILES = *.hex *.bin *.srec

# Rules for building the .eeprom rom images

eeprom: ehex ebin esrec

ehex:  $(PRG)_eeprom.hex
ebin:  $(PRG)_eeprom.bin
esrec: $(PRG)_eeprom.srec

%_eeprom.hex: %.elf
	$(OBJCOPY) -j .eeprom --change-section-lma .eeprom=0 -O ihex $< $@ \
	|| { echo empty $@ not generated; exit 0; }

%_eeprom.srec: %.elf
	$(OBJCOPY) -j .eeprom --change-section-lma .eeprom=0 -O srec $< $@ \
	|| { echo empty $@ not generated; exit 0; }

%_eeprom.bin: %.elf
	$(OBJCOPY) -j .eeprom --change-section-lma .eeprom=0 -O binary $< $@ \
	|| { echo empty $@ not generated; exit 0; }

burn : $(PRG).hex
	$(PRGMER) -p m16 -c $(PRGDEV) -e -U flash:w:$(PRG).hex