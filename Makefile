CFLAGS = -Wall -I./ -I./glfw/include/
# the include sucks. try some auto tools to make it simple. such as cmake
# the order matters, put -lglfw3 ahead of dependencies.
LDFLAGS = -L./glfw/src  -lGLEW  -lglfw3 -lGL -lXxf86vm -lXrandr -lpthread -lXi -lX11

LIB_DIR = /usr/lib/x86_64-linux-gnu
ifeq ($(shell getconf LONG_BIT), 32)
	LIB_DIR = /usr/lib/i386-linux-gnu
endif

SRC := \
main.cpp shader.cpp util.cpp lodepng/lodepng.c game_object.cpp map.cpp

ifeq (yes, $(DEBUG))
	CFLAGS += -DDEBUG_ENABLED -g
	SRC += log.cpp profiler.cpp

ifdef LOG_LEVEL
	CFLAGS += -DLOG_LEVEL=$(LOG_LEVEL)
LDFLAGS += -L$(LIB_DIR)
endif

ifeq (yes, $(PROFILER))
	CFLAGS += -DENABLE_PROFILING
endif

else

ifdef LOG_LEVEL
$(error "You must define DEBUG first to define LOG_LEVEL")
endif

ifdef PROFILER
$(error "You must define DEBUG first to define PROFILER")
endif

endif

TARGET := main
all : $(TARGET)

$(TARGET) : $(SRC)
	g++ $(CFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

clean :
	-rm -f main
