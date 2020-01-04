# Adapted from https://github.com/JMU-CS/c-test-framework/blob/master/ref/pT-blank/tests/Makefile

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

TEST=testsuite
LIBS=-lcheck -lm -lrt -lsubunit
UTESTOUT=utests.txt

all: test

$(EXE): $(OBJS)
	make -C ../

test: $(EXE) $(TEST)
	@echo "========================================"
	@echo "             UNIT TESTS                 "
	@./$(TEST) 2>/dev/null | sed -n -e '/Checks/,$$p' | sed -e 's/^private.*:[EF]://g'
	@echo "========================================"

$(TEST): $(TEST).o $(MODS) $(OBJS)
	$(CC) $(LDFLAGS) -o $(TEST) $^ $(LIBS)

$(OUTDIR)/%.o: $(SRCDIR)/%.c
	mkdir -p $(OUTDIR)
	$(CC) -c $(CFLAGS) $(INC) $< -o $@

%.o: %.c
	$(CC) -c $(CFLAGS) $(INC) $< -o $@

clean:
	rm -rf *.o testsuite utests.txt

.PHONY: all $(EXE) default clean
