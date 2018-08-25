BINARY=addons/sgp.so
STEAM=/home/nekz/.local/share/Steam/steamapps/common/
SDIR=src/
LDIR=lib/
ODIR=obj/

RPC=$(LDIR)discord-rpc/linux/libdiscord-rpc.a
CURL=$(LDIR)curl/linux/libcurl.a
CRYPTO=$(LDIR)openssl/libcrypto.a
SSL=$(LDIR)openssl/libssl.a

SRCS=$(wildcard $(SDIR)*.cpp)
SRCS+=$(wildcard $(SDIR)Modules/*.cpp)
SRCS+=$(wildcard $(SDIR)Portal2Boards/*.cpp)
SRCS+=$(wildcard $(SDIR)Portal2Boards/Entities/*.cpp)
SRCS+=$(wildcard $(SDIR)Portal2Boards/Entities/Aggregated/*.cpp)
SRCS+=$(wildcard $(SDIR)Portal2Boards/Entities/Chamber/*.cpp)
SRCS+=$(wildcard $(SDIR)Portal2Boards/Extensions/*.cpp)
OBJS=$(patsubst $(SDIR)%.cpp, $(ODIR)%.o, $(SRCS))

CC=g++
STFU=-Wno-unused-function -Wno-unused-variable -Wno-parentheses -Wno-unknown-pragmas
CFLAGS=-std=c++17 -m32 -fPIC -static-libstdc++ -shared -Wall $(STFU) -I$(LDIR) -I$(SDIR)
EXPORT=cp -fu
COPY=cp -n
CREATE=mkdir -p

all: pre sgp post

sgp: $(OBJS)
	$(CC) $(CFLAGS) -o $(BINARY) $^ $(RPC) $(CURL) $(CRYPTO) $(SSL) -lssl -lcrypto -ldl

$(ODIR)%.o: $(SDIR)%.cpp $(SDIR)%.hpp
	@echo $@
	@$(CC) $(CFLAGS) -c -o $@ $<

clean:
	@rm -rf $(ODIR)sgp.o $(BINARY)

clean-all:
	@rm -rf $(OBJS) $(BINARY)

pre:
	@$(CREATE) "$(STEAM)Portal 2/portal2/addons"
	@$(CREATE) "$(STEAM)Aperture Tag/aperturetag/addons"
	@$(CREATE) "$(STEAM)Portal Stories Mel/portal_stories/addons"
	@$(COPY) "addons/sgp.vdf" "$(STEAM)Portal 2/portal2/addons"
	@$(COPY) "addons/sgp.vdf" "$(STEAM)Aperture Tag/aperturetag/addons"
	@$(COPY) "addons/sgp.vdf" "$(STEAM)Portal Stories Mel/portal_stories/addons"
	@$(CREATE) $(ODIR)
	@$(CREATE) $(ODIR)Modules/
	@$(CREATE) $(ODIR)Portal2Boards/
	@$(CREATE) $(ODIR)Portal2Boards/Entities/
	@$(CREATE) $(ODIR)Portal2Boards/Entities/Aggregated/
	@$(CREATE) $(ODIR)Portal2Boards/Entities/Chamber/
	@$(CREATE) $(ODIR)Portal2Boards/Extensions/

post:
	@$(EXPORT) "$(BINARY)" "$(STEAM)Portal 2/portal2/$(BINARY)"
	@$(EXPORT) "$(BINARY)" "$(STEAM)Aperture Tag/aperturetag/$(BINARY)"
	@$(EXPORT) "$(BINARY)" "$(STEAM)Portal Stories Mel/portal_stories/$(BINARY)"
