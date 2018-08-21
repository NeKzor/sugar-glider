BINARY=addons/sgp.so
STEAM=/home/nekz/.local/share/Steam/steamapps/common/
SDIR=src/
LDIR=lib/
ODIR=obj/

RPC=/usr/local/lib/libdiscord-rpc.a
RC=/usr/local/lib/librestclient-cpp.a
CURL=/usr/local/lib/libcurl.a
CRYPTO=/usr/local/lib/libcrypto.a
SSL=/usr/local/lib/libssl.a

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

sgp: $(OBJS)
	$(CC) $(CFLAGS) -o $(BINARY) $^ $(RPC) $(RC) $(CURL) $(CRYPTO) $(SSL) -lssl -lcrypto -ldl
	@$(EXPORT) "$(BINARY)" "$(STEAM)Portal 2/portal2/$(BINARY)"

$(ODIR)%.o: $(SDIR)%.cpp $(SDIR)%.hpp
	@echo $@
	@$(CC) $(CFLAGS) -c -o $@ $<

clean:
	@rm -rf $(ODIR)sgp.o $(BINARY)

clean-all:
	@rm -rf $(OBJS) $(BINARY)
