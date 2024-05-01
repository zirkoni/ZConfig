CXX      = g++
CXXFLAGS = -g -O0 -MMD -std=c++17
LDFLAGS  = 
LIBS     =

INCLUDES+=-Isrc
INCLUDES+=-Iinclude

SRCS+=tests/*.cpp
SRCS+=src/*.cpp
CPPFLAGS+=$(INCLUDES)

cppsrc := $(wildcard $(SRCS))
obj    = $(cppsrc:.cpp=.o)
dep    = $(obj:.o=.d)

test_config: $(obj)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(LIBS)

test: test_config
	./test_config

.PHONY: clean test
clean:
	rm -f $(obj) test_config
	rm -f $(dep)

-include $(dep)
