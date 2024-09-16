TARGET := atlantis

CODE := 0x4000
# leaves 222 bytes for AKM player buffer
DATA := 0xc0de

# HEX, will fill with 0
ROM_MAX := 8000

OUTPUT := build

RDEV_PATH := F:\msxdevenv\bin

UBOX_PATH := $(RDEV_PATH)\ubox-msx-lib-master

HDR = \
	city.h\
	collision.h\
	difficulty.h\
	gorgons.h\
	hud.h\
	input.h\
	lane.h\
	queue.h\
	score.h\
	sprites.h\
	tiles.h\
	tileset.h\
	torpedo.h\
	wave.h\

SRC = \
	city.c\
	collision.c\
	difficulty.c\
	gorgons.c\
	hud.c\
	input.c\
	lane.c\
	queue.c\
	score.c\
	sprites.c\
	tiles.c\
	torpedo.c\
	wave.c\
	\
	main.c\

RELS := $(patsubst %.c, $(OUTPUT)/%.rel, $(SRC)) $(OUTPUT)/akm.rel
UBOX_LIBS := $(UBOX_PATH)\lib\ubox.lib $(UBOX_PATH)\lib\mplayer.lib
LIBS := -lubox -lmplayer

SDCC_PATH := $(RDEV_PATH)\sdcc-4.0.0-x64\bin
AS := $(SDCC_PATH)\sdasz80.exe
CC := $(SDCC_PATH)\sdcc.exe
CFLAGS := -mz80 --Werror -I$(UBOX_PATH)/include -I. --fsigned-char --std-sdcc99 --opt-code-speed
LDFLAGS := -L$(UBOX_PATH)/lib -L. --no-std-crt0# --fomit-frame-pointer


all: $(OUTPUT)/$(TARGET).rom
	echo "Done !!!"

openmsx: all
	openmsx -carta $(OUTPUT)/$(TARGET).rom -machine msx1

src/tileset.h: data\tiles.png
	python $(RDEV_PATH)\png2tiles.py -i tileset $< > $@

$(OUTPUT)/%.rel: src/%.c $(patsubst %.h, src/%.h, $(HDR))
	@if not exist "$(OUTPUT)" mkdir $(OUTPUT)
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

$(OUTPUT)/%.rel: src/%.z80
	@if not exist "$(OUTPUT)" mkdir $(OUTPUT)
	$(AS) -g -o $@ $<

$(OUTPUT)/akm.rel: src/akm.z80 src/song.asm src/effects.asm
	$(UBOX_PATH)\tools\rasm\rasm.exe src/akm.z80 -o $(OUTPUT)/akm -s -sl -sq
	$(RDEV_PATH)\Disark.exe --sourceProfile sdcc --symbolFile $(OUTPUT)/akm.sym --src16bitsValuesInHex --src8bitsValuesInHex --undocumentedOpcodesToBytes $(OUTPUT)/akm.bin $(OUTPUT)/akm_sdcc.asm
	$(AS) -g -o $@ $(OUTPUT)/akm_sdcc.asm

$(OUTPUT)/$(TARGET).rom: $(RELS) $(OUTPUT)/crt0.rel $(UBOX_LIBS)
	@if not exist "$(OUTPUT)" mkdir $(OUTPUT)
	$(CC) $(CFLAGS) $(LDFLAGS) $(LIBS) --code-loc $(CODE) --data-loc $(DATA) $(OUTPUT)/crt0.rel $(RELS) -o $(OUTPUT)/$(TARGET).ihx
	$(RDEV_PATH)\hex2bin.exe -e bin -p 00 -s 4000 -l $(ROM_MAX) $(OUTPUT)/$(TARGET).ihx
#	$(RDEV_PATH)\hex2bin.exe -e bin $(OUTPUT)/$(TARGET).ihx
	copy $(OUTPUT)\$(TARGET).bin $(OUTPUT)\$(TARGET).rom

clean:
	rmdir /S /Q $(OUTPUT)

.PHONY: all clean
