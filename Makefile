# src Makefile

include ../Makefile.common


all:$(patsubst %.c,../bulid/%.o,$(wildcard *.c))


../bulid/%.o:%.c
	$(CC) -c $< $(CFLAGS) -o $@

.PHONY: clean all

clean:
	rm -f ../bulid/*.o

