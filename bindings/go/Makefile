SHELL := /bin/bash

# Directories
LIBOQS_ROOT := ../..
BUILD_DIR := $(LIBOQS_ROOT)/build
CONFIG_DIR := .config

# Build configuration
CMAKE_FLAGS := -G Ninja -DBUILD_SHARED_LIBS=ON
NINJA_JOBS := 8

# Library paths
PKG_CONFIG_PATH_SET := export PKG_CONFIG_PATH=$(PWD)/$(CONFIG_DIR):$$PKG_CONFIG_PATH
DYLD_PATH_SET := export DYLD_LIBRARY_PATH=$(LIBOQS_ROOT)/build/lib:$$DYLD_LIBRARY_PATH
LD_PATH_SET := export LD_LIBRARY_PATH=$(LIBOQS_ROOT)/build/lib:$$LD_LIBRARY_PATH
ENV_SETUP := $(PKG_CONFIG_PATH_SET) && $(DYLD_PATH_SET) && $(LD_PATH_SET)

# Colors
GREEN := \033[0;32m
YELLOW := \033[1;33m
BLUE := \033[0;34m
NC := \033[0m

.PHONY: all clean liboqs go test example help

help:
	@echo -e "$(BLUE)liboqs NTT API Go Bindings - Build System$(NC)"
	@echo "=========================================="
	@echo ""
	@echo "Targets:"
	@echo "  make all        - Build liboqs C library + Go bindings"
	@echo "  make liboqs     - Build only liboqs C library"
	@echo "  make go         - Build only Go bindings"
	@echo "  make test       - Run Go tests"
	@echo "  make example    - Run Go example"
	@echo "  make clean      - Clean all build artifacts"
	@echo ""

all: liboqs go
	@echo -e "$(GREEN)✓ Build complete!$(NC)"
	@echo ""
	@echo "To use Go bindings, set these environment variables:"
	@echo "  export PKG_CONFIG_PATH=$(PWD)/$(CONFIG_DIR):\$$PKG_CONFIG_PATH"
	@echo "  export DYLD_LIBRARY_PATH=$(LIBOQS_ROOT)/build/lib:\$$DYLD_LIBRARY_PATH  # macOS"
	@echo "  export LD_LIBRARY_PATH=$(LIBOQS_ROOT)/build/lib:\$$LD_LIBRARY_PATH      # Linux"
	@echo ""

liboqs:
	@echo -e "$(YELLOW)Building liboqs C library...$(NC)"
	@if [ ! -f "$(BUILD_DIR)/build.ninja" ]; then \
		mkdir -p $(BUILD_DIR); \
		cd $(BUILD_DIR) && cmake $(CMAKE_FLAGS) ..; \
	fi
	@cd $(BUILD_DIR) && ninja -j$(NINJA_JOBS)
	@echo -e "$(GREEN)✓ liboqs built$(NC)"

go: liboqs
	@echo -e "$(YELLOW)Building Go bindings...$(NC)"
	@$(ENV_SETUP) && go clean -cache
	@$(ENV_SETUP) && cd ntt && go build
	@echo -e "$(GREEN)✓ Go bindings built$(NC)"

test: go
	@echo -e "$(YELLOW)Running tests...$(NC)"
	@$(ENV_SETUP) && cd ntt_test && go test -v
	@echo -e "$(GREEN)✓ Tests passed$(NC)"

example: go
	@echo -e "$(YELLOW)Running example...$(NC)"
	@$(ENV_SETUP) && cd examples && go run ntt_example.go
	@echo -e "$(GREEN)✓ Example completed$(NC)"

clean:
	@echo -e "$(YELLOW)Cleaning...$(NC)"
	@rm -f go.sum
	@cd ntt && go clean
	@cd ntt_test && go clean
	@cd examples && go clean
	@echo -e "$(GREEN)✓ Clean complete$(NC)"

clean-all: clean
	@echo -e "$(YELLOW)Cleaning liboqs build...$(NC)"
	@rm -rf $(BUILD_DIR)
	@rm -f go.mod
	@echo -e "$(GREEN)✓ Full clean complete$(NC)"

.PHONY: rebuild
rebuild: clean all
