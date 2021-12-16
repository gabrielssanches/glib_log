all: glib_log

SRC=
SRC+=main.c

GIO_FLAGS_L=$(shell pkg-config --libs gio-2.0 gio-2.0)
GIO_FLAGS_I=$(shell pkg-config --cflags-only-I gio-2.0)

OBJ=$(subst .c,.o,$(SRC))

glib_log: $(OBJ)
	gcc -ggdb3 -O0 $(OBJ) $(GIO_FLAGS_L) -o $@

clean:
	rm -f glib_log
	rm -f *.o
	rm -f *.d

DEPFLAGS = -MT $@ -MMD -MP -MF $*.d

%.o:%.c
	gcc -ggdb3 -O0 -c -std=gnu99 $(DEPFLAGS) $(GIO_FLAGS_I) $< -o $@

$(SRC): Makefile
	touch $@

-include $(wildcard $(SRC:.c=.d))
