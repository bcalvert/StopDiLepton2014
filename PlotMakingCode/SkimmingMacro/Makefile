ROOTLIBS=`root-config --ldflags --libs` -Wl,--no-as-needed -lGenVector
COMPFLAGS=`root-config --cflags` -Wall -Wextra -pedantic -Wno-vla -O2 -g -std=c++0x -DSTANDALONE_COMPILE
#COMPFLAGS=`root-config --cflags` -Wall -Wextra -pedantic -Wno-vla -O2 -std=c++0x -DSTANDALONE_COMPILE

all: NewOviStopPlotSkimmer

#StopPlotFiller:  StopDict_ver2.o StopPlotSkimmer.o
NewOviStopPlotSkimmer:  StopDict.o NewOviStopPlotSkimmer.o
	g++ -o $@ $^ $(ROOTLIBS) ../mt2bisect.cc

#StopDict_ver2.o: StopDict_ver2.cpp
StopDict.o: StopDictTry2.cpp
	g++ -c -o $@ $(COMPFLAGS) $^

NewOviStopPlotSkimmer.o: NewOviStopPlotSkimmer.C
	g++ -c -o $@ $(COMPFLAGS) $^

clean:
        rm StopDict.o NewOviStopPlotSkimmer.o