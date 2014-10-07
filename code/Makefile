LIB_PATH 			= /opt/local/lib/
LIB_NAME_RELEASE	= libfmodex.dylib
LIB_NAME_LOGGING	= libfmodexL.dylib

INC_DIRS			= -I/opt/local/include/
SOURCES				= main.cpp

CXX 				= clang++
CXX_FLAGS 			= -Wc++11-extensions


default: clean release

release: main.cpp
	$(CXX) -O3 -o main $(SOURCES) $(LIB_PATH)$(LIB_NAME_RELEASE) $(INC_DIRS) $(CXX_FLAGS)

debug: main.cpp
	$(CXX)-O0 -g -o main $(SOURCES) $(LIB_PATH)$(LIB_NAME_LOGGING) $(INC_DIRS) $(CXX_FLAGS)

clean:
	rm -f main