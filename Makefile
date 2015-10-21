
.PHONY: all

# remove ALL implit rules
MAKEFLAGS += " -r "
.SUFFIXES:

CXXFLAGS += -Wall -Weffc++ -Wextra -Werror -pedantic -std=c++11 -ggdb -D_GLIBCXX_DEBUG
CFLAGS += -Wall -Wextra -Werror -pedantic -std=gnu99 -ggdb
LDFLAGS += -ggdb

EXE_DIR = EXE
OBJ_DIR = OBJ

DIRS = $(EXE_DIR) $(OBJ_DIR)

CPPSOURCES += $(shell find * -type f -name "*.cpp")

CSOURCES += $(shell find * -type f -name "*.c")

OBJECTS += $(CPPSOURCES:%.cpp=$(OBJ_DIR)/%.o)

OBJECTS += $(CSOURCES:%.c=$(OBJ_DIR)/%.o)

EXES += $(CPPSOURCES:%.cpp=$(EXE_DIR)/%)
EXES += $(CSOURCES:%.c=$(EXE_DIR)/%)

all: $(EXES)

$(OBJ_DIR)/%.o: %.cpp | $(OBJ_DIR)
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	$(CC) -c $(CFLAGS) $< -o $@

$(EXE_DIR)/%: $(OBJ_DIR)/%.o | $(EXE_DIR)
	$(CXX) $(LDFLAGS) $< -o $@

$(DIRS):
	mkdir -p $@

clean:
	rm -f $(EXES)
	rmdir $(DIRS)
