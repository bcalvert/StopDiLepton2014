ROOTLIBS=`root-config --ldflags --libs` -Wl,--no-as-needed -lGenVector
COMPFLAGS=`root-config --cflags` -Wall -Wextra -pedantic -Wno-vla -O2 -g -std=c++0x -DSTANDALONE_COMPILE
#COMPFLAGS=`root-config --cflags` -Wall -Wextra -pedantic -O2 -g -DSTANDALONE_COMPILE

all: KK_GenPlotFiller

KK_GenPlotFiller:  KK_GenPlotFiller.o
	g++ -o $@ $^ $(ROOTLIBS)

KK_GenPlotFiller.o: KK_GenPlotFiller.C
	g++ -c -o $@ $(COMPFLAGS) $^

clean:
	rm KK_GenPlotFiller.o
