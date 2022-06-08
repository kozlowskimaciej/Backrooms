CXX = g++
# -fsanitize=undefined	- clang needed ?
CXX_FLAGS = -g -Wall -Wextra -pedantic -Werror -std=c++2a

BIN = bin
SRC = src
INCLUDE = include
TEMPLATE = template
LIB = lib
LIBRARIES = -lbox2d -lsfml-graphics -lsfml-window -lsfml-system

DIR = mains

CATCH = catch

MAIN = main
TEST = tests

MAIN_OBJ = $(BIN)/$(DIR)/$(MAIN).o
TEST_OBJ = $(BIN)/$(DIR)/$(TEST).o
CATCH_DIR = $(TEST)/catch2

SRCS = $(wildcard $(SRC)/*.cpp)							# source files in src/
OBJS = $(patsubst $(SRC)/%.cpp,$(BIN)/%.o,$(SRCS))		# obj files in bin/
HEADERS = $(INCLUDE)/*									# header files in include/
TEMPLATES = $(TEMPLATE)/*								# template implementation files in template/

all: compile link tests

link: $(OBJS) $(MAIN_OBJ)
	$(CXX) $(CXX_FLAGS) $(OBJS) $(MAIN_OBJ) -o $(BIN)/$(MAIN) -L$(LIB) $(LIBRARIES)    # $(MAIN) executable linkage

tests:	$(OBJS) $(TEST_OBJ)
	$(CXX) $(CXX_FLAGS) $(OBJS) $(TEST_OBJ) -o $(BIN)/$(TEST) -L$(LIB) $(LIBRARIES)    # $(TEST) executable linkage

compile: $(OBJS) $(MAIN_OBJ) $(TEST_OBJ)

$(BIN)/%.o: $(SRC)/%.cpp $(HEADERS) $(TEMPLATES) $(BIN)
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) -I$(TEMPLATE) -c $< -o $@    # $(SRC) files compilation
	
$(MAIN_OBJ): $(MAIN)/$(MAIN).cpp $(HEADERS) $(TEMPLATES) $(BIN) $(BIN)/$(DIR)
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) -I$(TEMPLATE) -c $< -o $@    # $(MAIN).cpp file compilation

$(TEST_OBJ): $(TEST)/$(TEST).cpp $(HEADERS) $(TEMPLATES) $(CATCH_DIR)/$(CATCH).hpp $(BIN) $(BIN)/$(DIR)
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) -I$(TEMPLATE) -I$(CATCH_DIR) -c $< -o $@    # $(TEST).cpp file compilation

$(BIN):
	mkdir $(BIN)    # $(BIN) directory creation

$(BIN)/$(DIR):
	mkdir $(BIN)/$(DIR)    # $(BIN)/$(DIR) directory creation

clean:
	rm -r $(BIN)    # removing $(BIN) and its contents