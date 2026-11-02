CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c11 -Iinclude -MMD -MP
LDFLAGS = 
DEVFLAGS = -fsanitize=address,undefined -g
RELFLAGS = -O3

DEP_DIRS := $(wildcard deps/*)
DEP_NAMES := $(notdir $(DEP_DIRS))

CFLAGS += $(foreach dir,$(DEP_DIRS),-I$(dir)/include)
LDFLAGS += $(foreach dir,$(DEP_DIRS),-L$(dir))
LDFLAGS += $(foreach name,$(DEP_NAMES),-l$(name))

DEP_LIBS := $(foreach name,$(DEP_NAMES),deps/$(name)/lib$(name).a)

LIB_A = libmatlib.a

SRC := $(wildcard src/*.c)
OBJ := $(SRC:.c=.o)

TEST_SRC := $(wildcard tests/*.c)
TEST_BIN := $(TEST_SRC:.c=)

DEPS = $(OBJ:.o=.d) $(TEST_SRC:.c=.d)

# Define ctest path (in the dependencies folder)
CTEST_PATH = deps/ctest/ctest

.PHONY: all clean test debug setup_deps

all: $(LIB_A)

debug: CFLAGS += $(DEVFLAGS)
debug: LDFLAGS += $(DEVFLAGS)
debug: all

$(LIB_A): $(OBJ)
		ar rcs $@ $(OBJ)

$(DEP_LIBS):
		$(MAKE) -C $(@D)

%.o: %.c
		$(CC) $(CFLAGS) -c -o $@ $<

setup_deps:
		git submodule update --init --recursive

test: CFLAGS += $(DEVFLAGS)
test: LDFLAGS += $(DEVFLAGS)
test: $(LIB_A) $(TEST_BIN)
		./$(CTEST_PATH)

tests/%: tests/%.c $(OBJ) $(DEP_LIBS)
		$(CC) $(CFLAGS) $< $(OBJ) $(LDFLAGS) -o $@

-include $(DEPS)

clean:
		rm -f $(OBJ) $(TEST_BIN) $(DEPS) $(LIB_A)

clean_deps: clean
		@for dir in $(DEP_DIRS); do $(MAKE) -C $$dir clean; done
