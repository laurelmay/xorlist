CC=gcc

override CFLAGS := -g -O0 -Wall -pedantic -std=c99 $(CFLAGS)
override LDFLAGS := -g -O0 $(LDFLAGS)

SRCDIR=../src
OUTDIR=../build
INCDIR=../include

TMPSRCS=$(shell find $(SRCDIR) -type f -name "*.c")
SRCS = $(filter-out $(SRCDIR)/main.c, $(TMPSRCS))
OBJS=$(patsubst $(SRCDIR)/%,$(OUTDIR)/%,$(SRCS:.c=.o))
INC=-I$(INCDIR)
MODS=tests.o

EXE=../list_test
TEST=testsuite
LIBS=-lcheck -lm -lrt -lsubunit

all: $(TEST)

$(EXE): $(OBJS)
	make -C ../

$(TEST): $(TEST).o $(MODS) $(OBJS)
	$(CC) $(LDFLAGS) -o $(TEST) $^ $(LIBS)

$(OUTDIR)/%.o: $(SRCDIR)/%.c
	mkdir -p $(OUTDIR)
	$(CC) -c $(CFLAGS) $(INC) $< -o $@

%.o: %.c
	$(CC) -c $(CFLAGS) $(INC) $< -o $@

clean:
	rm -rf *.o testsuite

.PHONY: all $(EXE) default clean
