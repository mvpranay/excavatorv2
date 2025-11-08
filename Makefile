CC = g++
CFLAGS = -std=c++11 -Wall
LDFLAGS = -lGL -lGLEW -lglfw -lm

TARGET = excavator
SOURCES = main.cpp gl_framework.cpp hierarchy_node.cpp keyframe.cpp
OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run