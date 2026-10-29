all: clean lib

lib:
	cc -g -o libmythread.so -shared -fPIC my_mutex.c cancel.c create.c exit.c join.c self.c threads_list.c

clean:
	rm -f libmythread.so

.PHONY: all lib clean
