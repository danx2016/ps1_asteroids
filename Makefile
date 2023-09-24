
TARGET = ps1_asteroids
TYPE = ps-exe

SRCS = third_party/nugget/common/crt0/crt0.s \
main.c \
game.c \
math.c \
vec2.c \
mem.c \
gfx.c \
audio.c \
input.c \
entity.c \
ship.c \
bullet_ship.c \
asteroid.c \
explosion.c \
third_party/nugget/modplayer/modplayer.c \
assets/musics/music.o \
assets/musics/music2.o \

CPPFLAGS += -Ithird_party/psyq-iwyu/include
LDFLAGS += -Lthird_party/psyq/lib
LDFLAGS += -Wl,--start-group
LDFLAGS += -lgcc
LDFLAGS += -lapi
LDFLAGS += -lc
LDFLAGS += -lc2
LDFLAGS += -lcard
LDFLAGS += -lcd
LDFLAGS += -lcomb
LDFLAGS += -lds
LDFLAGS += -letc
LDFLAGS += -lgpu
LDFLAGS += -lgs
LDFLAGS += -lgte
LDFLAGS += -lgun
LDFLAGS += -lhmd
LDFLAGS += -lmath
LDFLAGS += -lmcrd
LDFLAGS += -lmcx
LDFLAGS += -lpad
LDFLAGS += -lpress
LDFLAGS += -lsio
LDFLAGS += -lsnd
LDFLAGS += -lspu
LDFLAGS += -ltap
LDFLAGS += -Wl,--end-group

%.o: %.hit
	$(PREFIX)-objcopy -I binary --set-section-alignment .data=4 --rename-section .data=.rodata,alloc,load,readonly,data,contents -O $(FORMAT) -B mips $< $@

include third_party/nugget/common.mk
