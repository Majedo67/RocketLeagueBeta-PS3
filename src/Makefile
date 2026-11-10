TARGET := RocketLeagueBeta
TITLE := Rocket League Beta
APPID := RLBETA01
CONTENTID := UP0001-RLBETA01_00-0000000000000000

OFILES := main.o game.o

# Use the PS3 compiler driver so startup files and the C/C++ runtime are linked.
LD := ppu-g++

CFLAGS := -O2 -Wall \
-I$(PSL1GHT)/ppu/include

CXXFLAGS := -O2 -Wall -std=gnu++11 \
-I$(PSL1GHT)/ppu/include \
-I$(PSL1GHT)/ppu/include/simdmath \
-I$(PORTLIBS)/include

# PSL1GHT provides controller I/O, RSX/GCM, and video APIs through these libraries.
LIBS := -L$(PORTLIBS)/lib -ltiny3d -lio -lrsx -lgcm_sys -lsysutil -lm

include $(PS3DEV)/ppu_rules
