CXX=gcc

ROOTLIBS=`root-config --ldflags --libs` -Wl,--no-as-needed -lGenVector
COMPFLAGS=`root-config --cflags` -Wall -Wextra -pedantic -Wno-vla -O2 -g -std=c++0x -DSTANDALONE_COMPILE

SOURCES = $(wildcard *.cpp)
OBJS = $(addprefix ./, $(notdir $(SOURCES:.cpp=.o)))

OUT = libMT2.a

%.o: %.cpp
	$(CXX) $(COMPFLAGS) -c $< -o $@


$(OUT): $(OBJS)
	ar rcs $(OUT) $(OBJS)

.PHONY: clean
clean:
	rm -f $(OBJS) $(OUT)
