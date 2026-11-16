EE_BIN = build/smash-remix-ps2-hello.elf
EE_OBJS = main.o
EE_LIBS = -ldebug -lc

.PHONY: all clean

all: $(EE_BIN)

clean:
	rm -f $(EE_BIN) $(EE_OBJS)

include $(PS2SDK)/samples/Makefile.pref
include $(PS2SDK)/samples/Makefile.eeglobal
