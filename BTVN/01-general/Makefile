.PHONY: obj

PRJ_NAME := hello
SHARED_LIB_NAME := libhello.so
STATIC_LIB_NAME := libhello.a


CUR_DIR := $(shell pwd)
INC_DIR := $(CUR_DIR)/inc
SRC_DIR := $(CUR_DIR)/src
OBJ_DIR := $(CUR_DIR)/obj
BIN_DIR := $(CUR_DIR)/bin

LIB_DIR := $(CUR_DIR)/lib
LIB_STATIC_DIR := $(LIB_DIR)/static
LIB_SHARED_DIR := $(LIB_DIR)/shared

AR := ar
RCS := rcs
CC := gcc
C_FLAGS := -c -fPIC -o

mkdir_obj:
	if [ -d $(OBJ_DIR) ]; then \
		echo "dir is exist"; \
	else \
		mkdir $(OBJ_DIR); \
	fi

#create object file
obj: mkdir_obj
	$(CC) $(C_FLAGS) $(OBJ_DIR)/main.o $(CUR_DIR)/main.c -I $(INC_DIR)
	$(CC) $(C_FLAGS) $(OBJ_DIR)/hello_dai.o $(SRC_DIR)/hello_dai.c -I $(INC_DIR)
	$(CC) $(C_FLAGS) $(OBJ_DIR)/hello_world.o $(SRC_DIR)/hello_world.c -I $(INC_DIR)

#Create shared-lib
shared:
	$(CC) -shared $(OBJ_DIR)/* -o $(LIB_SHARED_DIR)/$(SHARED_LIB_NAME)

install:
	install $(LIB_SHARED_DIR)/$(SHARED_LIB_NAME) /usr/lib

all: obj shared 
	$(CC) $(OBJ_DIR)/main.o -L$(LIB_SHARED_DIR) -lhello -o $(BIN_DIR)/$(PRJ_NAME)
	LD_LIBRARY_PATH=$(LIB_SHARED_DIR) $(LD_LIBRARY_PATH)
	echo $(LD_LIBRARY_PATH)

#Create static lib
static: obj
	$(AR) $(RCS) $(LIB_STATIC_DIR)/$(STATIC_LIB_NAME) $(OBJ_DIR)/*

static_lib: static
	$(CC) $(OBJ_DIR)/main.o $(LIB_STATIC_DIR)/$(STATIC_LIB_NAME) -o $(BIN_DIR)/$(PRJ_NAME)

clean:
	rm -rf $(OBJ_DIR)/*
	rm -rf $(LIB_SHARED_DIR)/$(SHARED_LIB_NAME)
	rm -rf $(BIN_DIR)/*
	rm -rf $(LIB_STATIC_DIR)/$(STATIC_LIB_NAME)
