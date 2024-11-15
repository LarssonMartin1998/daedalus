# Variables
BUILD_TESTS ?= ON
BUILD_TYPE ?= Debug
BUILD_DIR = build
CMAKE = cmake
CMAKE_GENERATOR ?= Ninja
CMAKE_FLAGS = -G $(CMAKE_GENERATOR) -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) -DBUILD_TESTS=$(BUILD_TESTS)
CONAN = conan
CONAN_FLAGS = install atlas --output-folder=$(BUILD_DIR) --build=missing -s build_type=$(BUILD_TYPE)
CTEST = ctest

# All target
.PHONY: all # Don't run this as go-to during development, only intended as a CI target/clean build.
all: clean setup build analyze test

.DEFAULT_GOAL := build

# Setup target
.PHONY: setup
setup:
	@echo "Setting up the build environment..."
	@ln -sf $(BUILD_DIR)/compile_commands.json compile_commands.json
	@ln -sf atlas/.clang-format .clang-format
	@ln -sf atlas/.clang-tidy .clang-tidy
	@mkdir -p $(BUILD_DIR)
	@$(CONAN) $(CONAN_FLAGS)
	@cd $(BUILD_DIR) && $(CMAKE) $(CMAKE_FLAGS) ..
	@echo "Environment setup complete."

# Build target
.PHONY: build
build:
	@echo "Building the project..."
	@$(CMAKE) --build $(BUILD_DIR) -- -j $(shell nproc)
	@echo "Build complete."

# Run target
.PHONY: run
run:
	@echo "Running Daedalus..."
	@echo "-------------------"
	@$(BUILD_DIR)/daedalus
	@echo "-------------------"
	@echo "Daedalus finished"

# Build and run target
.PHONY: build_run
build_run: build run

# Test target
.PHONY: test
test:
	@echo "Running tests..."
	@cd $(BUILD_DIR) && $(CTEST) --output-on-failure
	@echo "Tests complete."

# Analyze target
.PHONY: analyze
analyze:
	@echo "Analyzing the project..."
	@scan-build $(MAKE) build
	@echo "Analysis complete."

# Clean target
.PHONY: clean
clean:
	@echo "Cleaning up..."
	@rm -rf $(BUILD_DIR) CMakeUserPresets.json .cache compile_commands.json .clang-format .clang-tidy
	@cd atlas && make clean
	@echo "Cleanup complete."
