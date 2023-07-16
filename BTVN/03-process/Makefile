CC := gcc
C_FLAGS := -c -fPIC -o

lib_c=$(wildcard ../lib/*.c)
lib_o=$(lib_c:.c=.o)

cur_c=$(wildcard *.c)
cur_o=$(cur_c:.c=.o)


../lib/%.o: ../lib/%.c 
	$(CC) $(C_FLAGS) $@ $^

%.o: %.c 
	$(CC) $(C_FLAGS) $@ $^

fork: $(lib_o) fork.o 
	$(CC) $^ -o $@

ass1: ass1.o 
	$(CC) $^ -o $@

all: fork ass1

clean: 
	rm -rf $(lib_o)
	rm -rf $(cur_o)
	rm -rf ass1 fork

.PHONY: clean fork