TARGET := lib/liblat-ifc.a
OBJS := $(patsubst src/%.c, build/%.o, $(wildcard src/*.c))

LIBLOKI_DIR ?= /usr/groups/comparch-loki/tools/releases/libloki/current

$(TARGET): $(OBJS) | lib
	loki-elf-ar rc $@ $+
	loki-elf-ranlib $@

build/%.o: src/%.c $(wildcard include/lat/*.h) | build
	loki-clang -O3 -Iinclude -I$(LIBLOKI_DIR)/include -c -Werror -Wall -o $@ $<

.PHONY: clean
clean:
	rm -f $(wildcard $(TARGET) *.o)
	rm -rf $(wildcard lib build)

lib:
	mkdir $@
build:
	mkdir $@
