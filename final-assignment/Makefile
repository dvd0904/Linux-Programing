EXTERNAL_JSON=cJSON/
EXTERNAL_SQLITE=sqlite/

AR 		:= ar
CC 		:= gcc
C_FLAGS :=-I./ -I./headers/ -I$(EXTERNAL_JSON) -pthread 
LINK	:= ${AR} -rcs
RANLIB  := ranlib

JSON_LIB   	 = $(EXTERNAL_JSON)libcjson.a
SQLITE_LIB   = $(EXTERNAL_SQLITE)libsqlite3.a

shared_c := $(wildcard shared/*.c)
shared_o := $(shared_c:.c=.o)

sqlite_c = ${EXTERNAL_SQLITE}sqlite3.c
sqlite_o = ${EXTERNAL_SQLITE}sqlite3.o

cjson_c := ${EXTERNAL_JSON}cJSON.c
cjson_o := $(cjson_c:.c=.o)

os_net_c := $(wildcard os_net/*.c)
os_net_o := $(os_net_c:.c=.o)

# dai: ${shared_o} ${os_net_o} dai.c $(JSON_LIB)
# 	${CC} ${C_FLAGS} $^ -o $@ 

all: main client 

main: ${shared_o} ${os_net_o} $(JSON_LIB) $(SQLITE_LIB) main-server.c
	${CC} ${C_FLAGS} $^ -o $@ 

client: ${shared_o} ${os_net_o} client.c $(JSON_LIB)
	${CC} ${C_FLAGS} $^ -o $@ 



.PHONY: main client dll dai


shared/%.o: shared/%.c
	${CC} ${C_FLAGS} -fPIC -c $^ -o $@

$(JSON_LIB): ${cjson_o}
	${LINK} $@ $^
	${RANLIB} $@

${EXTERNAL_JSON}%.o: ${EXTERNAL_JSON}%.c
	${CC} ${C_FLAGS} -fPIC -c $^ -o $@

$(SQLITE_LIB): $(sqlite_o)
	${LINK} $@ $^
	${RANLIB} $@

$(sqlite_o): $(sqlite_c)
	${CC} ${C_FLAGS} -w -fPIC -c $^ -o $@ 


os_net/%.o: os_net/%.c
	${CC} ${C_FLAGS} -fPIC -c $^ -o $@



clean:
	rm -rf ${shared_o}
	rm -rf main
	rm -rf client
	rm -rf ${cjson_o}
	rm -rf ${sqlite_o}
	rm -rf ${os_net_o}
	rm -rf $(JSON_LIB)
	rm -rf $(SQLITE_LIB)