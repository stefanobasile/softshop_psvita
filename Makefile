TITLE_ID = VITA2DTST
TARGET   = softshop
PROJECT_TITLE := softshop

OBJS     = utils.o ime_dialog.o ui_editbox.o ui_button.o main.o \
           main_screen.o \
           ui_helper.o \
           fornitori_screen.o \
           image.o background.o \
           selected.o pressed.o \
           b1_normal.o b1_pressed.o b1_selected.o \
           b2_normal.o b2_pressed.o b2_selected.o \
           b3_normal.o b3_pressed.o b3_selected.o \
           b4_normal.o b4_pressed.o b4_selected.o \
           b5_normal.o b5_pressed.o b5_selected.o \
           b6_normal.o b6_pressed.o b6_selected.o \
           b7_normal.o b7_pressed.o b7_selected.o \
           b8_normal.o b8_pressed.o b8_selected.o

LIBS = -lvita2d -lSceDisplay_stub -lSceGxm_stub \
	-lSceSysmodule_stub -lSceCtrl_stub -lScePgf_stub -lScePvf_stub \
	-lSceCommonDialog_stub -lSceTouch_stub -lfreetype -lpng -ljpeg -lz -lm -lc \
    -lSceAppMgr_stub -lSceAppUtil_stub \
    -lSceAudio_stub -lSceAudiodec_stub


PREFIX  = arm-vita-eabi
CC      = $(PREFIX)-gcc
CFLAGS  = -Wl,-q -Wall -fno-lto
ASFLAGS = $(CFLAGS)

all: $(TARGET).vpk

%.vpk: eboot.bin
	vita-mksfoex -d PARENTAL_LEVEL=1 -s APP_VER=01.00 -s TITLE_ID=$(TITLE_ID) "$(PROJECT_TITLE)" param.sfo
	vita-pack-vpk -s param.sfo -b eboot.bin \
		--add sce_sys/icon0.png=sce_sys/icon0.png \
		--add sce_sys/livearea/contents/startup.png=sce_sys/livearea/contents/startup.png \
		--add sce_sys/livearea/contents/template.xml=sce_sys/livearea/contents/template.xml \
    softshop.vpk

eboot.bin: $(TARGET).velf
	vita-make-fself -s $< $@

%.velf: %.elf
	vita-elf-create $< $@

$(TARGET).elf: $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

%.o: %.png
	$(PREFIX)-ld -r -b binary -o $@ $^

clean:
	@rm -rf $(TARGET).vpk $(TARGET).velf $(TARGET).elf $(OBJS) \
		eboot.bin param.sfo

vpksend: $(TARGET).vpk
	curl -T $(TARGET).vpk ftp://$(PSVITAIP):1337/ux0:/
	@echo "Sent."

send: eboot.bin
	curl -T eboot.bin ftp://$(PSVITAIP):1337/ux0:/app/$(TITLE_ID)/
	@echo "Sent."
