CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -pedantic
SRCS := src/main.cpp src/utils.cpp src/install.cpp src/dns_query.cpp src/help.cpp
OBJS := $(SRCS:.cpp=.o)
TARGET := dns-tool

ifeq ($(TERMUX),1)
    PREFIX ?= $(HOME)/../usr
else
    PREFIX ?= /usr/local
endif
DESTDIR ?=
BINDIR := $(DESTDIR)$(PREFIX)/bin

.PHONY: all clean install uninstall

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

install: $(TARGET)
	install -d $(BINDIR)
	install -m 755 $(TARGET) $(BINDIR)/$(TARGET)

uninstall:
	rm -f $(BINDIR)/$(TARGET)
