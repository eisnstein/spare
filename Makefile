CC = gcc
CFLAGS = -g -O1 -Wall -Wextra -pedantic -Isrc -std=c11
LDLIBS = 

BUILD_DIR ?= ./build
SRC_DIR ?= ./src
TEST_DIR ?= ./tests
TARGET = $(BUILD_DIR)/spare

SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SOURCES))

SOURCES_TESTS = $(wildcard $(TEST_DIR)/*_test.c)
TESTS = $(patsubst %.c,%,$(SOURCES_TESTS))

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $^ -o $@ $(CFLAGS) $(LDLIBS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) -c $^ -o $@ $(CFLAGS)

.PHONY: release
release: CFLAGS += -DNDEBUG
release: all

.PHONY: tests
tests: $(TESTS)
	./runtests.sh

$(TEST_DIR)/example_test: $(TEST_DIR)/example_test.c
	$(CC) $< -o $@ $(CFLAGS)

.PHONY: clean
clean:
	rm -f $(OBJECTS) $(OBJECTS_TESTS) $(TESTS) 
	rm -rf ./build
	rm -f ./tests/error_log.txt
	rm -f ./*.o

