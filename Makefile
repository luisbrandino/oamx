SHELL := /usr/bin/bash
CC = gcc

SRC_DIR = src
TEST_DIR = tests
BUILD_DIR = build

TESTS = $(wildcard $(TEST_DIR)/*.c)
TEST_BINS = $(patsubst $(TEST_DIR)/%.c,$(BUILD_DIR)/%,$(TESTS))

compile_tests: $(TEST_BINS)

$(BUILD_DIR)/%: $(TEST_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	@$(CC) -o $@ $< $(filter-out $(SRC_DIR)/main.c, $(wildcard $(SRC_DIR)/*.c))

tests: compile_tests
	@echo === Running all tests ===
	@$(foreach t,$(TEST_BINS), \
		echo -n "-> $(t) ... "; \
		if $(t); then \
			echo "Passed"; \
		else \
			echo "Failed"; \
			exit 1; \
		fi; \
	)
	@echo === All tests concluded ===

all:
	gcc src/main.c src/mbc.c -o oamx.exe

clean:
	@rm -rf build

.PHONY: all tests compile_tests clean