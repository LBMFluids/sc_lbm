# Compile tests for Geometry class
vpath %.h ../common ../../include #../../include/geom_object
vpath %.cpp ../common ../../src #../../src/geom_object

CXX=g++
CXXFLAGS=-std=c++11
TARGET=geom_tests

OBJS= geometry_tests.o test_utils.o geometry.o

$(TARGET) : $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) r5_test.cpp $(OBJS)

geometry.o: geometry.cpp geometry.h
	$(CXX) -c $(CXXFLAGS) ../../src/geometry.cpp 
test_utils.o: test_utils.cpp test_utils.h
	$(CXX) -c $(CXXFLAGS) ../common/test_utils.cpp
geometry_tests.o: geometry_tests.cpp geometry_tests.h
	$(CXX) -c $(CXXFLAGS) geometry_tests.cpp

.PHONY : clean
clean:
	rm -f *.o

