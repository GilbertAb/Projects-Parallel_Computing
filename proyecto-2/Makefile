APPNAME=forest
HEADERS=$(wildcard src/*.hpp)
SOURCES=$(wildcard src/*.cpp)
EXECUTABLES=$(APPNAME) $(APPNAME)_asan $(APPNAME)_msan $(APPNAME)_tsan $(APPNAME)_ubsan
LINTFILTERS=$\
	-build/c++11,$\
	-build/header_guard,$\
	-build/include_subdir,$\
	-readability/casting,$\
	-runtime/references

CC=mpic++ -cxx=g++
CC2=mpic++ -cxx=clang++
CFLAGS=-g -Wall -Wextra -fopenmp -std=gnu++11
CFLAGS2= -O3 -std=gnu++11
LIBS=

all: directory $(EXECUTABLES)

$(APPNAME): $(HEADERS) $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o bin/$(APPNAME) $(LIBS)

$(APPNAME)_asan:  $(HEADERS) $(SOURCES)
	$(CC2) $(CFLAGS) -fsanitize=address $(SOURCES) -o bin/$(APPNAME)_asan $(LIBS)

$(APPNAME)_msan:  $(HEADERS) $(SOURCES)
	$(CC2) $(CFLAGS) -fsanitize=memory $(SOURCES) -o bin/$(APPNAME)_msan $(LIBS)

$(APPNAME)_tsan:  $(HEADERS) $(SOURCES)
	$(CC2) $(CFLAGS) -fsanitize=thread $(SOURCES) -o bin/$(APPNAME)_tsan $(LIBS)

$(APPNAME)_ubsan:  $(HEADERS) $(SOURCES)
	$(CC2) $(CFLAGS) -fsanitize=undefined $(SOURCES) -o bin/$(APPNAME)_ubsan $(LIBS)

.PHONY: release
release:
	mkdir -p bin
	$(CC) $(CFLAGS2) $(SOURCES) -o bin/$(APPNAME) $(LIBS)

.PHONY: directory
directory:
	mkdir	-p bin
	
.PHONY: memcheck
memcheck:
	valgrind ./bin/$(APPNAME)

.PHONY: helgrind
helgrind:
	valgrind --tool=helgrind --quiet ./bin/$(APPNAME)

.PHONY: rebuild
rebuild: clean $(APPNAME)

.PHONY: lint
lint:
	cpplint --filter=$(LINTFILTERS) $(HEADERS) $(SOURCES)

.PHONY: gitignore
gitignore:
	echo $(EXECUTABLES) | tr " " "\n" > .gitignore

.PHONY: clean
clean:
	rm -rf bin