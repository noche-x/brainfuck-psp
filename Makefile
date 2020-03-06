TARGET = brainfuck-psp
OBJS = src/main.o \
src/common.o \

INCDIR = $(PSPPATH)/include
CFLAGS = -O3 -G3 -ffast-math -fno-rtti -fno-exceptions -mpreferred-stack-boundary=4
CXXFLAGS = -std=gnu++17
ASFLAGS = $(CFLAGS)

BUILD_PRX = 1
PSP_FW_VERSION=500
PSP_LARGE_MEMORY = 1 #PHAT SUPPORT! Our app isn't that big! 

LDFLAGS =
# -losl -lmmio -lpng

LIBS    = -lpspusb -lpspusbstor -lpspmp3 -lmad -lpspvram -lpspreg -lpspumd -lpsphprm -lpspaudiolib -lpspaudio -lmikmod -lpspaudiocodec -lpng -lz -lpspgum -lpspgu -lstdc++ -lpsprtc -lpspctrl -lm -lpspvfpu -lpsppower -lpsphttp -ljpeg
# LIBS = -lpspaudiolib -lpspaudio -lmikmod -lpspaudiocodec -lpng -lz -lpspgum -lpspgu -lstdc++ -lpsprtc -lpspctrl -lm -lpspvfpu -lpspsdk -lpsppower -lpsphttp

EXTRA_TARGETS = EBOOT.PBP
# PSP_EBOOT_ICON= ICON0.png
PSP_EBOOT_TITLE = Brainfuck PSP

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak
