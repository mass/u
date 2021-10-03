CXX       := g++
CXXFLAGS  := -std=c++17 -g -ggdb -Wall -Werror -Wextra -Wconversion -Wpedantic -pedantic-errors -O2
DEPFLAGS   = -MT $@ -MMD -MP -MF $*.d
INCLUDE   := -I../include
BINS      := m stream_buf log
DEPFILES  := $(BINS:%=%.d)

.PHONY: all
all : $(BINS)

.PHONY: clean
clean :
	rm -f *.o *.d $(BINS)

# Remove default rule, add rule with dependency file
%.o : %.cpp
%.o : %.cpp %.d
	$(CXX) $(DEPFLAGS) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

$(BINS) : % : %.o
	$(CXX) $(CXXFLAGS) -o $@ $<

# Add targets for each dependency file
$(DEPFILES) :

# Include auto-generated dependency rules
include $(wildcard $(DEPFILES))
