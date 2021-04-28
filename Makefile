# Makefile for Pelz
PREFIX = /usr/local

CC = gcc -std=c11 #-DTIMEOUTSEC=1 -DTIMEOUTUSEC=0
DEBUG = -g

INCLUDES = 
LLIBS = 
CFLAGS = -Wall -c $(DEBUG) -D_GNU_SOURCE $(INCLUDES) 
LFLAGS = -Wall $(DEBUG) -lcrypto -lssl -pthread -lcjson -lkmyth-logger


OBJ_DIR = objs
SRC_DIR = src

INCLUDE_DIR = include

PELZ_SRC_DIR = $(SRC_DIR)/pelz
PELZ_OBJ_DIR = $(OBJ_DIR)/pelz

UTIL_SRC_DIR = $(SRC_DIR)/util
UTIL_OBJ_DIR = $(OBJ_DIR)/util

HEADER_FILES = $(wildcard $(INCLUDE_DIR)/*h)

PELZ_SOURCES = $(wildcard $(PELZ_SRC_DIR)/*c)
PELZ_OBJECTS = $(subst $(PELZ_SRC_DIR), $(PELZ_OBJ_DIR), $(PELZ_SOURCES:%.c=%.o))

UTIL_SOURCES = $(wildcard $(UTIL_SRC_DIR)/*c)
UTIL_OBJECTS = $(subst $(UTIL_SRC_DIR), $(UTIL_OBJ_DIR), $(UTIL_SOURCES:%.c=%.o))

OBJECTS= $(PELZ_OBJECTS) $(UTIL_OBJECTS)

all: pre pelz

pelz: $(PELZ_OBJECTS) $(UTIL_OBJECTS)
	$(CC) $(PELZ_OBJECTS) $(UTIL_OBJECTS) -o bin/pelz $(LLIBS) $(LFLAGS)

pre:
	indent -bli0 -bap -bad -sob -cli0 -npcs -nbc -bls -blf -nlp -ip0 -ts2 -nut -npsl -bbo -l128 src/*/*.c
	indent -bli0 -bap -bad -sob -cli0 -npcs -nbc -bls -blf -nlp -ip0 -ts2 -nut -npsl -bbo -l128 include/*.h
	rm -f src/*/*.c~
	rm -f include/*.h~
	mkdir -p bin

docs: $(HEADER_FILES) $(CRYPRO_SOURCES) Doxyfile
	doxygen Doxyfile 

$(PELZ_OBJ_DIR)/%.o: $(PELZ_SRC_DIR)/%.c | $(PELZ_OBJ_DIR) 
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

$(UTIL_OBJ_DIR)/%.o: $(UTIL_SRC_DIR)/%.c | $(UTIL_OBJ_DIR) 
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

$(PELZ_OBJ_DIR):
	mkdir -p $(OBJ_DIR) $(PELZ_OBJ_DIR)
	
$(UTIL_OBJ_DIR):
	mkdir -p $(OBJ_DIR) $(UTIL_OBJ_DIR)

.PHONY: install
install:
	mkdir -p $(PREFIX)/bin
	cp bin/pelz $(PREFIX)/bin
	chmod 711 $(PREFIX)/bin/pelz
	mkdir -p /var/log

.PHONY: uninstall
uninstall:
	rm -f $(PREFIX)/bin/pelz
	rm -f var/log/pelz.log

clean:
	-rm -fr $(OBJECTS) bin/pelz