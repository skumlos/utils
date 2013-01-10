TOP = .
SRCS = $(shell ls *.cpp)
OBJS = $(SRCS:%.cpp=%.o)
LIB = lib$(notdir $(CURDIR)).a

lib: $(LIB)

$(LIB): $(OBJS)
	ar -rcs $@ $?

clean:
	rm -f *.a *.o
