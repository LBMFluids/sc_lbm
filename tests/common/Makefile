# Compile tests for common testing utilities
CXX=g++
CXXFLAGS=-std=c++11
TARGET=tu_test

OBJS=test_utils.o

$(TARGET) : $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) test_utils_test.cpp $(OBJS)

test_utils.o: test_utils.cpp test_utils.h
	$(CXX) -c $(CXXFLAGS) test_utils.cpp

.PHONY : clean
clean:
	rm -f *.o

