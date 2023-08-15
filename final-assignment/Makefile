EXTERNAL_JSON=cJSON/

AR 		:= ar
CC 		:= gcc
C_FLAGS :=-I./ -I./headers/ -I$(EXTERNAL_JSON) -pthread 
LINK	:= ${AR} -rcs
RANLIB  := ranlib

JSON_LIB   	 = $(EXTERNAL_JSON)libcjson.a

shared_c := $(wildcard shared/*.c)
shared_o := $(shared_c:.c=.o)

shared/%.o: shared/%.c
	${CC} ${C_FLAGS} -fPIC -c $^ -o $@

cjson_c := ${EXTERNAL_JSON}cJSON.c
cjson_o := $(cjson_c:.c=.o)


$(JSON_LIB): ${cjson_o}
	${LINK} $@ $^
	${RANLIB} $@

${EXTERNAL_JSON}%.o: ${EXTERNAL_JSON}%.c
	${CC} ${C_FLAGS} -fPIC -c $^ -o $@

main: ${shared_o} main-server.c $(JSON_LIB)
	${CC} ${C_FLAGS} $^ -o $@

.PHONY: main

clean:
	rm -rf ${shared_o}
	rm -rf main
	rm -rf ${cjson_o}
	rm -rf $(JSON_LIB)