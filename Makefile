CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -pedantic
SRCS := src/main.cpp src/utils.cpp src/install.cpp src/dns_query.cpp src/help.cpp
OBJS := $(SRCS:.cpp=.o)
TARGET := dns-tool

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
