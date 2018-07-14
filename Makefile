CC=gcc

override CFLAGS := -Wall -pedantic -std=c99 $(CFLAGS)
override LDFLAGS := $(LDFLAGS)

SRCS=list.c main.c
OBJS=$(SRCS:.c=.o)

TARGET_LIB=list.o
EXE=list_test

all: $(EXE)

$(EXE): $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -rf *.o list_test

.PHONY: all default clean
