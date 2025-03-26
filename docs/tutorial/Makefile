.PHONY: all install uninstall clean

CC := gcc
CFLAGS := -Wall -Wextra -O2
TARGETS := cfs edf fifo lock thread-affinity
PREFIX ?= 
BINDIR := $(PREFIX)/bin

all: $(TARGETS)

%: %.c
	$(CC) $(CFLAGS) -o $@ $<

install: $(TARGETS)
	mkdir -p $(DESTDIR)$(BINDIR)
	install -m 755 $(TARGETS) $(DESTDIR)$(BINDIR)

uninstall:
	rm -f $(addprefix $(DESTDIR)$(BINDIR)/, $(TARGETS))

clean:
	rm -f $(TARGETS)