BUILD_TESTS ?= ON
BUILD_TYPE ?= Debug
BUILD_DIR = build
CMAKE = cmake
CMAKE_GENERATOR ?= Ninja
CMAKE_FLAGS = -G $(CMAKE_GENERATOR) -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) -DBUILD_TESTS=$(BUILD_TESTS)
CTEST = ctest

.DEFAULT_GOAL := build

.PHONY: generate
generate:
	@echo "Generating the project..."
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && $(CMAKE) $(CMAKE_FLAGS) ..
	@echo "Generation complete."

.PHONY: setup
setup:
	@echo "Setting up the build environment..."
	@ln -sf $(BUILD_DIR)/compile_commands.json compile_commands.json
	@ln -sf atlas/.clang-format .clang-format
	@ln -sf atlas/.clang-tidy .clang-tidy
	@mkdir -p $(BUILD_DIR)
	@make generate
	@echo "Environment setup complete."

.PHONY: build
build:
	@echo "Building the project..."
	@$(CMAKE) --build $(BUILD_DIR) -- -j $(shell nproc)
	@echo "Build complete."

.PHONY: run
run:
	@echo "Running Daedalus..."
	@echo "-------------------"
	@$(BUILD_DIR)/daedalus
	@echo "-------------------"
	@echo "Daedalus finished"

.PHONY: build_run
build_run: build run

.PHONY: test
test:
	@echo "Running tests..."
	@cd $(BUILD_DIR) && $(CTEST) --output-on-failure
	@echo "Tests complete."

.PHONY: analyze
analyze:
	@echo "Analyzing the project..."
	@scan-build $(MAKE) build
	@echo "Analysis complete."

.PHONY: clean
clean:
	@echo "Cleaning up..."
	@rm -rf $(BUILD_DIR) CMakeUserPresets.json .cache compile_commands.json .clang-format .clang-tidy result
	@cd atlas && make clean
	@echo "Cleanup complete."
