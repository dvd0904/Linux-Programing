EXTERNAL_JSON=cJSON/

AR 		:= ar
CC 		:= gcc
C_FLAGS :=-I./ -I./headers/ -I$(EXTERNAL_JSON) -pthread 
LINK	:= ${AR} -rcs
RANLIB  := ranlib

JSON_LIB   	 = $(EXTERNAL_JSON)libcjson.a

shared_c := $(wildcard shared/*.c)
shared_o := $(shared_c:.c=.o)

cjson_c := ${EXTERNAL_JSON}cJSON.c
cjson_o := $(cjson_c:.c=.o)

os_net_c := $(wildcard os_net/*.c)
os_net_o := $(os_net_c:.c=.o)

all: main client

main: ${shared_o} ${os_net_o} main-server.c $(JSON_LIB)
	${CC} ${C_FLAGS} $^ -o $@ 

client: ${shared_o} ${os_net_o} client.c $(JSON_LIB)
	${CC} ${C_FLAGS} $^ -o $@ 

.PHONY: main client


shared/%.o: shared/%.c
	${CC} ${C_FLAGS} -fPIC -c $^ -o $@

$(JSON_LIB): ${cjson_o}
	${LINK} $@ $^
	${RANLIB} $@

${EXTERNAL_JSON}%.o: ${EXTERNAL_JSON}%.c
	${CC} ${C_FLAGS} -fPIC -c $^ -o $@


os_net/%.o: os_net/%.c
	${CC} ${C_FLAGS} -c $^ -o $@



clean:
	rm -rf ${shared_o}
	rm -rf main
	rm -rf client
	rm -rf ${cjson_o}
	rm -rf ${os_net_o}
	rm -rf $(JSON_LIB)