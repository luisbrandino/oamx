SHELL := /usr/bin/bash
CC = gcc
CXX = g++
SRC_DIR = src
TEST_DIR = tests
BUILD_DIR = build
DEBUGGER_DIR = src/debugger

GLFW_DIR = C:/dev/glfw
GLFW_INC = -I$(GLFW_DIR)/include
GLFW_LIB = -L$(GLFW_DIR)/lib-mingw-w64 -lglfw3 -lopengl32 -lgdi32

IMGUI_DIR = C:/dev/imgui
IMGUI_BACKENDS = $(IMGUI_DIR)/backends

# SDL2
CFLAGS = -IC:/dev/sdl2/include
CXXFLAGS = -IC:/dev/sdl2/include -Iinc -I$(DEBUGGER_DIR) \
           -I$(IMGUI_DIR) -I$(IMGUI_BACKENDS) $(GLFW_INC)
LDFLAGS  = -LC:/dev/sdl2/lib -lSDL2
LIBS    = -lmingw32 -lSDL2main -lSDL2 -lstdc++

CORE_SRCS = $(wildcard $(SRC_DIR)/*.c)
DBG_SRCS  = $(wildcard $(DEBUGGER_DIR)/*.cpp)

CORE_OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(CORE_SRCS))
DBG_OBJS  = $(patsubst $(DEBUGGER_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(DBG_SRCS)) \
            $(patsubst $(IMGUI_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(IMGUI_SRCS))

IMGUI_SRCS = $(IMGUI_DIR)/imgui.cpp \
             $(IMGUI_DIR)/imgui_draw.cpp \
             $(IMGUI_DIR)/imgui_tables.cpp \
             $(IMGUI_DIR)/imgui_widgets.cpp \
             $(IMGUI_BACKENDS)/imgui_impl_glfw.cpp \
             $(IMGUI_BACKENDS)/imgui_impl_opengl3.cpp

OBJS = $(CORE_OBJS) $(DBG_OBJS)

TESTS = $(wildcard $(TEST_DIR)/*.c)
TEST_BINS = $(patsubst $(TEST_DIR)/%.c,$(BUILD_DIR)/%,$(TESTS))

compile_tests: $(TEST_BINS)

$(BUILD_DIR)/%: $(TEST_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	@$(CC) -o $@ $< $(filter-out $(SRC_DIR)/main.c $(SRC_DIR)/display.c, $(wildcard $(SRC_DIR)/*.c))

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

all: oamx.exe

oamx.exe: $(OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CXX) -o $@ $(OBJS) $(LDFLAGS) $(LIBS) $(GLFW_LIB)

# Compila objetos C
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Compila objetos C++
$(BUILD_DIR)/%.o: $(DEBUGGER_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# (Opcional) compila IMGUI
$(BUILD_DIR)/%.o: $(IMGUI_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@rm -rf $(BUILD_DIR) $(SRC_DIR)/*.o $(DEBUGGER_DIR)/*.o oamx.exe

.PHONY: all tests compile_tests clean