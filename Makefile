CC=clang

override CFLAGS := -Wall -pedantic -std=c17 $(CFLAGS)
override LDFLAGS := $(LDFLAGS)

SRCDIR = src
OUTDIR = build
INCDIR = include
PREFIX = /usr

SRCS = $(shell find $(SRCDIR) -type f -name *.c)
OBJS = $(patsubst $(SRCDIR)/%,$(OUTDIR)/%,$(SRCS:.c=.o))
SHOBJS = $(patsubst $(SRCDIR)/%,$(OUTDIR)/lib%,$(SRCS:.c=.so))
INC  = -I$(INCDIR)

EXE=list_test

all: libs

libs: $(OBJS) $(SHOBJS)

test:
	make -C tests

exe: $(EXE)

$(EXE): $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

$(OUTDIR)/%.o: $(SRCDIR)/%.c
	mkdir -p $(OUTDIR)
	$(CC) -c $(CFLAGS) $(INC) $< -o $@

$(OUTDIR)/lib%.so: $(OUTDIR)/%.o
	$(CC) $(CFLAGS) -shared -o $@ $<

install: $(SHOBJS)
	install -d $(DESTDIR)$(PREFIX)/lib/
	install -m 644 $(SHOBJS) $(DESTDIR)$(PREFIX)/lib/
	install -d $(DESTDIR)$(PREFIX)/include/xorlist
	install -m 644 $(INCDIR)/* $(DESTDIR)$(PREFIX)/include/xorlist/

clean:
	rm -rf $(OUTDIR) list_test
	make -C tests clean

.PHONY: all default clean tests install
