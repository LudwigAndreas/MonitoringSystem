# Compiler settings
CXX = g++
# CXXFLAGS = -std=c++17 -Wall -Wextra -Werror -Ofast -march=native -pedantic -fPIC
CXXFLAGS = -std=c++17 -fPIC -fvisibility=hidden -g -O0

# Source files
SRCS_FILES = cpu.cc

SRCS = $(notdir $(SRCS_FILES))

# Object files
OBJS = $(patsubst %.cc,%.o,$(SRCS))

# Library settings
LIB_PREFIX = ms
LIB_NAME = cpu
LIB_DIR = ..
LIB = $(LIB_DIR)/lib$(LIB_PREFIX)$(LIB_NAME).so

all: $(LIB)

$(LIB): $(OBJS)
	@$(CXX) -shared $(OBJS) -o $(LIB)
	@echo "\033[1;33m""$(LIB) is up to date."'\033[0m'

%.o: %.cc
	@$(CXX) $(CXXFLAGS) -c -o $@ $<
	@printf "\033[1;36m/\033[0m"

clean:
	@$(RM) $(OBJS)
	@echo '\033[1;31m'"$(LIB) deleted."'\033[0m'

fclean: clean
	@$(RM) $(LIB)
	@echo '\033[1;31m'"$(LIB) objs deleted."'\033[0m'

re: fclean all

.PHONY: all clean fclean re bonus
