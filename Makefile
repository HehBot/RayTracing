CXX := g++
CC := gcc
TARGET_NAME := main

BUILD_DIR := build
BIN_DIR := bin
SRC_DIR := src
LIB_DIR :=

TARGET_EXEC = $(BIN_DIR)/$(TARGET_NAME)

SRCS := $(shell find $(SRC_DIR) -name '*.cc' -or -name '*.c')
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

INC_FLAGS := -I$(SRC_DIR)

CXXFLAGS := -Wall -Wpedantic -Werror -g $(INC_FLAGS) -MMD -MP -O3
CCFLAGS := -Wall -Wpedantic -Werror -g $(INC_FLAGS) -MMD -MP -O3
LDFLAGS :=
LIBFLAGS := -lpthread

.PHONY: clean

#1 2 3 4 5 6 7 8 9 10: $(TARGET_EXEC)
#	./$< img$@.ppm $@

$(TARGET_EXEC): $(OBJS)
	@mkdir -p $(dir $@)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LIBFLAGS)

$(BUILD_DIR)/%.cc.o: %.cc
	@mkdir -p $(dir $@)
	$(CXX) -c $(CXXFLAGS) -o $@ $<

$(BUILD_DIR)/%.c.o: %.c
	@mkdir -p $(dir $@)
	$(CC) -c $(CCFLAGS) -o $@ $<

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

-include $(DEPS)