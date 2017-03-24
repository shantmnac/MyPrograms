CXX=g++
CXXFLAGS= -Wall -g -std=c++11 -pg

OBJ_FILES=$(patsubst %.cpp, %.o, $(wildcard *.cpp))


BIN_FILES=out

all: $(BIN_FILES)

out: $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o : %.cpp %.h
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -rf $(OBJ_FILES) $(BIN_FILES)

