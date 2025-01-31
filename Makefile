# leave empty for release
DEBUG := 1

CXX := g++
CXXFLAGS := -fPIC -std=c++20 -Iinclude $(if $(DEBUG),-Wall -Wextra -pedantic -g,-O3)
LDFLAGS := -lfmt
MODULES := giga/app giga/archive giga/bytestream giga/endianness giga/lzss giga/platform
TARGET := libgiga.so

SOURCEDIR := src
SOURCES := $(foreach MODULE,$(MODULES),$(SOURCEDIR)/$(MODULE).cpp)

OBJECTDIR := build
OBJECTS := $(foreach MODULE,$(MODULES),$(OBJECTDIR)/$(MODULE).o)

all: $(OBJECTS)

$(OBJECTS): $(OBJECTDIR)/%.o: $(SOURCEDIR)/%.cpp
	$(CXX) $< $(CXXFLAGS) -c -o $@

$(TARGET): $(OBJECTS)
	$(CXX) -shared -fPIC -o $(TARGET) $(OBJECTS)

install: $(TARGET)
	cp -r include/giga /usr/include
	cp $(TARGET) /usr/lib

uninstall:
	rm -rf /usr/include/giga /usr/lib/$(TARGET)

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: all clean
