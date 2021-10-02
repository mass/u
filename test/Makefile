CXX       := g++
CXXFLAGS  := -std=c++17 -g -ggdb -Wall -Werror -Wextra -Wconversion -Wpedantic -pedantic-errors -O2
BINS      := stream_buf log
INCLUDE   := -I../include

#TODO: Add header deps

.PHONY: all
all: $(BINS)

%.o : %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

$(BINS): % : %.o
	$(CXX) $(CXXFLAGS) $(INCLUDE) -o $@ $<

.PHONY: clean
clean:
	rm -f *.o $(BINS)
