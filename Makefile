# Makefile for CMake Project

# CMake configuration
CURRENT_DIR := $(shell pwd)
BUILD_DIR := build
BIN_DIR := bin
DIST_DIR := dist
SOURCE_DIR := src
TEST_DIR := test
COVERAGE_DIR := coverage
CMAKE := cmake
CMAKE_FLAGS := -DCMAKE_BUILD_TYPE=Debug
# CMAKE_FLAGS := -DCMAKE_BUILD_TYPE=Release

# Targets
TARGET := MonitoringSystem

# Documentation
DOXYFILE = Doxyfile
DOXYGEN := doxygen

# Distribution files
DIST_FILES := $(SOURCE_DIR) $(TEST_DIR) $(DOXYFILE) CMakeLists.txt Makefile README.md

# Style
CLANG_FORMAT := clang-format
CPP_EXTENSIONS := cpp cc cxx c++ h hpp hxx h++ inc inl
SRC_DIRS := $(SOURCE_DIR) $(TEST_DIR) third_party/LogLite third_party/libs21
CPP_FILES := $(shell find $(SRC_DIRS) -type f -name "*.$(CPP_EXTENSIONS)")

.PHONY: all build test install uninstall dvi dist clean fclean gcov re bonus

# Build rules
all: build

build: $(BUILD_DIR)/CMakeCache.txt
	@$(MAKE) -C $(BUILD_DIR) $(TARGET)
	@echo "Build complete. Run './$(BIN_DIR)/$(TARGET)' to execute."

$(BUILD_DIR)/CMakeCache.txt:
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && $(CMAKE) $(CMAKE_FLAGS) $(CURRENT_DIR)

test: $(BUILD_DIR)/CMakeCache.txt
	@$(MAKE) -C $(BUILD_DIR) init
	@$(MAKE) -C $(BUILD_DIR) $(TARGET)-unittests
	@echo "Build complete. Run './$(BIN_DIR)/MonitoringSystem-unittests' to execute."

install: $(BUILD_DIR)/CMakeCache.txt
	@$(MAKE) -C $(BUILD_DIR) install

uninstall: $(BUILD_DIR)/CMakeCache.txt
	@$(MAKE) -C $(BUILD_DIR) uninstall

dvi: $(BUILD_DIR)/CMakeCache.txt
	@$(DOXYGEN) $(DOXYFILE)
	@cd latex && $(MAKE) dvi
	@cd latex && mv refman.dvi ../your_documentation.dvi
	@echo "Build complete. Run 'xdvi $(BUILD_DIR)/doc/latex/refman.dvi' to execute."

dist: clean $(BUILD_DIR)/CMakeCache.txt
	@mkdir -p $(DIST_DIR)
	@cp -r $(DIST_FILES) $(DIST_DIR)/
	@tar -czvf $(BUILD_DIR)/$(TARGET)-$(shell date +%Y%m%d).tar.gz $(DIST_DIR)
	@rm -rf $(DIST_DIR)
	@echo "Build complete. Run 'tar -xzvf $(BUILD_DIR)/$(TARGET)-$(shell date +%Y%m%d).tar.gz' to execute."

check-style:
	@cp $(CURRENT_DIR)/materials/linters/.clang-format .
	@for file in $(CPP_FILES); do \
	    $(CLANG_FORMAT) -n "$$file"; \
	done
	@rm ./.clang-format
	@echo "Code style check complete."

clean:
	@$(MAKE) -C $(BUILD_DIR) clean
	@#rm -rf $(BUILD_DIR)

fclean: clean
	@rm -rf $(BIN_DIR)
	@rm -rf $(BUILD_DIR)
	@rm -rf latex
	@rm -rf html
	@rm -rf $(COVERAGE_DIR)
	@rm -rf coverage.info

gcov: test
	./$(BIN_DIR)/$(TARGET)-unittests
	lcov --directory . --capture --output-file coverage.info
	lcov --remove coverage.info '/usr/*' \
								'/Applications/Xcode.app/*' \
								'v1/' \
								"$(CURRENT_DIR)/third_party/*" \
								 "$(CURRENT_DIR)/$(BUILD_DIR)/*" \
								  "$(CURRENT_DIR)/$(BIN_DIR)/*" \
								   "$(CURRENT_DIR)/code-samples/*" \
								    "$(CURRENT_DIR)lib/*" \
								     "$(CURRENT_DIR)materials/*" \
								      --output-file coverage.info
	genhtml coverage.info --output-directory $(COVERAGE_DIR)
	open $(COVERAGE_DIR)/index.html

leaks:

re: fclean all
