CFLAGS = -g -Wall -I./ -I./glfw/include/
# the include sucks. try some auto tools to make it simple like cmake
LDFLAGS = -L/usr/lib/x86_64-linux-gnu -L./glfw/src  -lGLEW  -lglfw3 -lGL -lXxf86vm -lXrandr -lpthread -lXi

SRC := \
main.cpp

TARGET := main
all : $(TARGET)

$(TARGET) : $(SRC)
	g++ $(CFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

clean :
	-rm -f main
