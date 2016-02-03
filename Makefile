CC = g++
MKDIR_P = mkdir -p
CFLAGS = -Iinclude/ -std=c++11 -Wall -pedantic -O3 -fopenmp
CPP_FILES := $(wildcard src/*.cpp)
OBJ_DIR := obj
OBJ_FILES := $(addprefix ${OBJ_DIR}/,$(notdir $(CPP_FILES:.cpp=.o)))
SOURCE_FOLDER = src/

ACO_TSP_Solver: $(OBJ_FILES)
	${CC} -fopenmp -o $@ $^

${OBJ_DIR}/%.o:src/%.cpp
	${CC} ${CFLAGS} -c -o  $@ $<

$(OBJ_FILES): | $(OBJ_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

format:
	astyle -rn --style=kr  --indent=spaces=4 --indent-switches --indent-cases --break-blocks --convert-tabs --add-brackets --align-pointer=type *.cpp *.hpp

clean:
	${RM} -r ACO_TSP_Solver obj
