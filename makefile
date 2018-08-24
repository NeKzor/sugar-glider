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

all: dirs | sgp

sgp: $(OBJS)
	$(CC) $(CFLAGS) -o $(BINARY) $^ $(RPC) $(CURL) $(CRYPTO) $(SSL) -lssl -lcrypto -ldl
	@$(EXPORT) "$(BINARY)" "$(STEAM)Portal 2/portal2/$(BINARY)"

$(ODIR)%.o: $(SDIR)%.cpp $(SDIR)%.hpp
	@echo $@
	@$(CC) $(CFLAGS) -c -o $@ $<

clean:
	@rm -rf $(ODIR)sgp.o $(BINARY)

clean-all:
	@rm -rf $(OBJS) $(BINARY)

dirs:
	@mkdir -p $(ODIR)
	@mkdir -p $(ODIR)Modules/
	@mkdir -p $(ODIR)Portal2Boards/
	@mkdir -p $(ODIR)Portal2Boards/Entities/
	@mkdir -p $(ODIR)Portal2Boards/Entities/Aggregated/
	@mkdir -p $(ODIR)Portal2Boards/Entities/Chamber/
	@mkdir -p $(ODIR)Portal2Boards/Extensions/
