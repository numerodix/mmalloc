.PHONY: build-tests run-tests tests clean

CFLAGS := -std=gnu99 -Wall -Werror -g


bin/allocate_once_and_fill: tests/allocate_once_and_fill.c src/mmalloc.c include/mmalloc.h
	@mkdir -p bin
	cc $(CFLAGS) -I include -o $@ tests/allocate_once_and_fill.c src/mmalloc.c

bin/free_zero: tests/free_zero.c src/mmalloc.c include/mmalloc.h
	@mkdir -p bin
	cc $(CFLAGS) -I include -o $@ tests/free_zero.c src/mmalloc.c

bin/allocate_twice: tests/allocate_twice.c src/mmalloc.c include/mmalloc.h
	@mkdir -p bin
	cc $(CFLAGS) -I include -o $@ tests/allocate_twice.c src/mmalloc.c

bin/allocate_zero: tests/allocate_zero.c src/mmalloc.c include/mmalloc.h
	@mkdir -p bin
	cc $(CFLAGS) -I include -o $@ tests/allocate_zero.c src/mmalloc.c


build-tests: bin/allocate_once_and_fill bin/free_zero bin/allocate_twice bin/allocate_zero

run-tests:
	./bin/allocate_once_and_fill
	./bin/free_zero
	./bin/allocate_twice
	./bin/allocate_zero


tests: build-tests run-tests

clean:
	rm -rf bin

