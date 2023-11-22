CCCOLOR="\033[34m"
LINKCOLOR="\033[34;1m"
SRCCOLOR="\033[33m"
BINCOLOR="\033[37;1m"
MAKECOLOR="\033[32;1m"
ENDCOLOR="\033[0m"

QUIET_CC      = @printf '    %b %b\n' ${CCCOLOR}CC${ENDCOLOR} ${SRCCOLOR}$@${ENDCOLOR} 1>&2;
QUIET_LINK    = @printf '    %b %b\n' ${LINKCOLOR}LINK${ENDCOLOR} ${BINCOLOR}$@${ENDCOLOR} 1>&2;
QUIET_CCBIN   = @printf '    %b %b\n' ${LINKCOLOR}CC${ENDCOLOR} ${BINCOLOR}$@${ENDCOLOR} 1>&2;
QUIET_INSTALL = @printf '    %b %b\n' ${LINKCOLOR}INSTALL${ENDCOLOR} ${BINCOLOR}$@${ENDCOLOR} 1>&2;
QUIET_RANLIB  = @printf '    %b %b\n' ${LINKCOLOR}RANLIB${ENDCOLOR} ${BINCOLOR}$@${ENDCOLOR} 1>&2;
QUIET_NOTICE  = @printf '%b' ${MAKECOLOR} 1>&2;
QUIET_ENDCOLOR= @printf '%b' ${ENDCOLOR} 1>&2;


EXTERNAL_JSON=cJSON/
EXTERNAL_SQLITE=sqlite/

AR 		:= ar
CC 		:= gcc
C_FLAGS :=-I./ -I./headers/ -I./os_net/ -I./fdb/ -I$(EXTERNAL_JSON) -I$(EXTERNAL_SQLITE) -pthread 
LD_FLAGS := -ldl
LINK	:= ${AR} -rcs
RANLIB  := ranlib

F_CC      =${QUIET_CC}${CC}
F_CCBIN   =${QUIET_CCBIN}${CC}
F_LINK    =${QUIET_LINK}${AR} -crs
F_RANLIB  =${QUIET_RANLIB}${RANLIB}

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

fdb_sql := $(wildcard fdb/*.sql)
fdb_c := $(wildcard fdb/*.c)
fdb_o := $(fdb_c:.c=.o) $(fdb_sql:.sql=.o)

all: main client 

main: ${fdb_o} ${shared_o} ${os_net_o} $(JSON_LIB) $(SQLITE_LIB) main-server.c
	${F_CCBIN} ${C_FLAGS} $^ -o $@ ${LD_FLAGS}

client: ${shared_o} ${os_net_o} client.c $(JSON_LIB)
	${F_CCBIN} ${C_FLAGS} $^ -o $@ 

.PHONY: main client dll dai


shared/%.o: shared/%.c
	${F_CC} ${C_FLAGS} -fPIC -c $^ -o $@

$(JSON_LIB): ${cjson_o}
	${F_LINK} $@ $^
	${F_RANLIB} $@

${EXTERNAL_JSON}%.o: ${EXTERNAL_JSON}%.c
	${F_CC} ${C_FLAGS} -fPIC -c $^ -o $@

$(SQLITE_LIB): $(sqlite_o)
	${F_LINK} $@ $^
	${F_RANLIB} $@

$(sqlite_o): $(sqlite_c)
	${F_CC} ${C_FLAGS} -w -fPIC -c $^ -o $@ 


os_net/%.o: os_net/%.c
	${F_CC} ${C_FLAGS} -fPIC -c $^ -o $@

fdb/schema_%.o: fdb/schema_%.sql
	${QUIET_CC}echo 'const char *'$(word 2, $(subst /, ,$(subst .,_,$<))) '= "'"`cat $< | tr -d \"\n\"`"'";' | ${CC} ${C_FLAGS} -xc -c -o $@ -

fdb/%.o: fdb/%.c
	${F_CC} ${C_FLAGS} -fPIC -c $^ -o $@


clean:
	rm -rf ${shared_o}
	rm -rf main
	rm -rf client
	rm -rf ${cjson_o}
	rm -rf ${sqlite_o}
	rm -rf ${os_net_o}
	rm -rf $(JSON_LIB)
	rm -rf ${fdb_o}
	rm -rf $(SQLITE_LIB)