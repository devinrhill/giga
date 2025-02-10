# leave empty for release
DEBUG :=

CXX := g++
CXXFLAGS := -fPIC -std=c++20 -Iinclude $(if $(DEBUG),-Wall -Wextra -pedantic -g,-O3)
LDFLAGS :=
MODULES := archive bytestream endianness lzss platform
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
	cp -r include /usr/include/giga
	cp $(TARGET) /usr/lib

uninstall:
	rm -rf /usr/include/giga /usr/lib/$(TARGET)

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: all clean
