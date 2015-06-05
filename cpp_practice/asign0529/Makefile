CC=g++
SOURCE=camera.cpp
OBJECTS=$(SOURCE: .cpp=.o)
CFLAGS=-Wall 
TARGET=camera
LIBS=$(-lm pkg-config --clfags --libs opencv)
$(TAREGT): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS) $(LIBS)
.SUFFIXES: .o  .cpp
.cpp.o:
	$(CC) $(FLAGS) -c $? -o $@
camera.o: camera.cpp
	g++ -c camera.cpp
clean:
	rm camera camera.o
