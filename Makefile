CC=clang

override CFLAGS := -Wall -pedantic -std=c23 $(CFLAGS)
override LDFLAGS := $(LDFLAGS)

SRCDIR = src
OUTDIR = build
INCDIR = include
PREFIX = /usr
DOCDIR = docs
DOCS   = $(DOCDIR)/html/index.html
CSSURL = https://raw.githubusercontent.com/jothepro/doxygen-awesome-css/main/doxygen-awesome.css
CSSFILE = doxygen-awesome.css

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

docs: $(DOCS)

$(CSSFILE):
	curl -sL -o $(CSSFILE) $(CSSURL)

$(DOCS): $(OBJS) $(SHOBJS) $(CSSFILE)
	@doxygen

clean:
	rm -rf $(OUTDIR) list_test docs/
	make -C tests clean

.PHONY: all default clean tests install
