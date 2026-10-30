CC      = cc
CFLAGS  = -g
LDFLAGS = -lmythread -L.
LIBS    = libmythread.so

TEST_SRCS := $(wildcard t/*.c)

all: clean test

test: $(LIBS)
	$(CC) $(CFLAGS) test.c $(TEST_SRCS) -o test $(LDFLAGS)

$(LIBS):
	make -C ../lib
	mv ../lib/libmythread.so ./

clean:
	rm -f $(LIBS)
	make -C ../lib clean

.PHONY: all clean test
