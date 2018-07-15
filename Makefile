CC=gcc

override CFLAGS := -Wall -pedantic -std=c99 $(CFLAGS)
override LDFLAGS := $(LDFLAGS)

SRCDIR = src
OUTDIR = build
INCDIR = include

SRCS = $(shell find $(SRCDIR) -type f -name *.c)
OBJS = $(patsubst $(SRCDIR)/%,$(OUTDIR)/%,$(SRCS:.c=.o))
INC  = -I$(INCDIR)

EXE=list_test

all: $(EXE)

$(EXE): $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

$(OUTDIR)/%.o: $(SRCDIR)/%.c
	mkdir -p $(OUTDIR)
	$(CC) -c $(CFLAGS) $(INC) $< -o $@

clean:
	rm -rf $(OUTDIR)/*.o list_test

.PHONY: all default clean
