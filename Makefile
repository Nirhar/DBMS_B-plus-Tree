#!Makefile

BUILD=build
SOURCE=src
INCLUDE=include
TEST=test
TEST_SRC=$(TEST)/src
TEST_BUILD=$(TEST)/build

targets: $(BUILD)/node.o $(BUILD)/b+tree.o

$(BUILD)/node.o: $(SOURCE)/node.cpp $(INCLUDE)/node.hpp
	g++ -c $(SOURCE)/node.cpp -o $@ -g

$(BUILD)/b+tree.o: $(SOURCE)/b+tree.cpp $(INCLUDE)/b+tree.hpp $(INCLUDE)/node.hpp
	g++ -c $(SOURCE)/b+tree.cpp -o $@ -g

test:$(TEST_BUILD)/test_node.exe $(TEST_BUILD)/test_tree.exe

$(TEST_BUILD)/test_node.exe: $(TEST_SRC)/test_node.cpp $(INCLUDE)/node.hpp $(BUILD)/node.o
	g++ $(TEST_SRC)/test_node.cpp $(BUILD)/node.o -I$(INCLUDE) -o $@ -g

$(TEST_BUILD)/test_tree.exe: $(TEST_SRC)/test_tree.cpp $(INCLUDE)/b+tree.hpp  $(BUILD)/b+tree.o
	g++ $(TEST_SRC)/test_tree.cpp $(BUILD)/b+tree.o $(BUILD)/node.o -I$(INCLUDE) -o $@ -g

all: targets test 

clean:
	del /q $(BUILD)\*.o $(TEST)\build\*.exe
