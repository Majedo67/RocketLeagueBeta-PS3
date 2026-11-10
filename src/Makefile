TARGET := RocketLeagueBeta
TITLE := Rocket League Beta
APPID := RLBETA1
CONTENTID := UP0001-RLBETA1_00-0000000000000000

OFILES := main.o game.o

CFLAGS := -O2 -Wall \
-I$(PSL1GHT)/ppu/include

CXXFLAGS := -O2 -Wall -std=gnu++11 \
-I$(PSL1GHT)/ppu/include \
-I$(PSL1GHT)/ppu/include/simdmath \
-I$(PORTLIBS)/include

LIBS := -L$(PORTLIBS)/lib -ltiny3d

include $(PS3DEV)/ppu_rules
