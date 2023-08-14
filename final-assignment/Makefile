AR := ar
RCS := rcs
CC := gcc
C_FLAGS :=-I./ -I./headers/ -pthread

shared_c := $(wildcard shared/*.c)
shared_o := $(shared_c:.c=.o)

shared/%.o: shared/%.c
	${CC} ${C_FLAGS} -fPIC -c $^ -o $@

main: ${shared_o} main.c
	${CC} ${C_FLAGS} $^ -o $@


clean:
	rm -rf ${shared_o}
	rm -rf main