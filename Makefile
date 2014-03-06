CFLAGS = -g -Wall -I./ -I./glfw/include/
# the include sucks. try some auto tools to make it simple. such as cmake
# the order matters, put -lglfw3 ahead of dependencies.
LDFLAGS = -L./glfw/src  -lGLEW  -lglfw3 -lGL -lXxf86vm -lXrandr -lpthread -lXi -lX11

LIB_DIR = /usr/lib/x86_64-linux-gnu
ifeq ($(shell getconf LONG_BIT), 32)
	LIB_DIR = /usr/lib/i386-linux-gnu
endif

LDFLAGS += -L$(LIB_DIR)

SRC := \
main.cpp shader.cpp

TARGET := main
all : $(TARGET)

$(TARGET) : $(SRC)
	g++ $(CFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

clean :
	-rm -f main
