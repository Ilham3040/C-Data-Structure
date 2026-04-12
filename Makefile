CC = gcc
CFLAGS = -std=c23 -I./data_structure -I./utilities -Wall -Wextra -g

# 1. Segregate your sources
CORE_SRCS = $(wildcard data_structure/*.c)
APP_MAIN  = main.c
TEST_SRCS = $(wildcard test/*.c)

OUT_DIR = output
APP_TARGET  = $(OUT_DIR)/app
TEST_TARGET = $(OUT_DIR)/test_suite

ifeq ($(V),1)
  Q =
else
  Q = @
endif

# Default: Build the actual app
all: $(APP_TARGET)

# Build App: Uses CORE files + main.c
$(APP_TARGET): $(CORE_SRCS) $(APP_MAIN)
	$(Q) mkdir -p $(OUT_DIR)
	$(Q) $(CC) $(CFLAGS) $^ -o $@

# Build Test: Uses CORE files + everything in test/
$(TEST_TARGET): $(CORE_SRCS) $(TEST_SRCS)
	$(Q) mkdir -p $(OUT_DIR)
	$(Q) $(CC) $(CFLAGS) $^ -o $@

# Run the test
test: $(TEST_TARGET)
	$(Q)./$(TEST_TARGET)

clean:
	$(Q) rm -rf $(OUT_DIR)

.PHONY: all test clean