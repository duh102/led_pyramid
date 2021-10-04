LDSCRIPT = xdk-asf/samd21e15l_flash.ld
BOOTUP = xdk-asf/startup_samd21.o xdk-asf/system_samd21.o
MCUTYPE=__SAMD21E15L__

APP=main

PATTERNS=$(wildcard patterns/*.c)

OBJS=$(BOOTUP) $(APP).o $(PATTERNS:%.c=%.o) hsv_rgb.o

# Tools
CC=arm-none-eabi-gcc
LD=arm-none-eabi-gcc
AR=arm-none-eabi-ar
AS=arm-none-eabi-as
OBJCOPY=arm-none-eabi-objcopy
SIZE=arm-none-eabi-size

ELF=$(APP).elf

LDFLAGS+= -T$(LDSCRIPT) -mthumb -mcpu=cortex-m0 -Wl,--gc-sections
CFLAGS+= -mcpu=cortex-m0 -mthumb -Os
CFLAGS+= -I xdk-asf -I xdk-asf/include -I xdk-asf/cmsis -I . -I patterns
CFLAGS+= -D$(MCUTYPE)

$(APP).hex: $(APP).elf
	$(OBJCOPY) -O ihex $< $@
	$(SIZE) $< | size-parse --micro samd21e15l

$(ELF): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS)

# compile and generate dependency info
%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@
	$(CC) -MM $(CFLAGS) $< > $*.d

%.o:    %.s
	$(AS) $< -o $@

clean:
	rm -f $(OBJS) $(OBJS:.o=.d) $(ELF) startup_stm32f* $(CLEANOTHER) $(APP).hex

debug:  $(ELF)
	arm-none-eabi-gdb -iex "target extended-remote localhost:3333" $(ELF)

# pull in dependencies
-include        $(OBJS:.o=.d)
