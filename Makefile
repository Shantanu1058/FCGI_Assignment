# Directory

SRC_DIR=src
OBJ_DIR=obj
INC_DIR=-I.

#Variables
CC=gcc
GCC_FLAGS= -Wall -Werror -Wextra $(INC_DIR)
LFLAGS= -lfcgi -ggdb -O3 -lmemcached -lpthread
MYSQL_CONFIG= $$(mysql_config --cflags --libs) 
TARGET=fcgi_app
OBJECTS = $(patsubst $(SRC_DIR)/phase4.c,$(OBJ_DIR)/phase4.o, $(wildcard $(SRC_DIR)/phase4.c))


#Main
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -c -o $@ $< $(INC_DIR) $(MYSQL_CONFIG) $(LFLAGS)
        

all ::  $(TARGET)

$(TARGET) :  $(OBJECTS)
	$(CC) -o $@ $^ $(LFLAGS) $(GCC_FLAGS) $(MYSQL_CONFIG)

run :	$(TARGET)
	spawn-fcgi -a 127.0.0.1 -p 8080 $<
