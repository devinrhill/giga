# leave empty for release
DEBUG := 1

CXX := g++
CXXFLAGS := -std=c++20 -Iinclude $(if $(DEBUG),-Wall -Wextra -pedantic -g,-O3)
LDFLAGS := -lfmt
MODULES := giga/app giga/archive giga/bytestream giga/endianness giga/lzss giga/platform

SOURCEDIR := src
SOURCES := $(foreach MODULE,$(MODULES),$(SOURCEDIR)/$(MODULE).cpp)

OBJECTDIR := build
OBJECTS := $(foreach MODULE,$(MODULES),$(OBJECTDIR)/$(MODULE).o)

all: $(OBJECTS)

$(OBJECTS): $(OBJECTDIR)/%.o: $(SOURCEDIR)/%.cpp
	$(CXX) $< $(CXXFLAGS) -c -o $@

clean:
	rm -f $(OBJECTS)

.PHONY: all clean
