CXX = g++
CXXFLAGS = -g -Wall -fPIC
ROOTFLAGS = `root-config --cflags --glibs`

# make a binary for every .cxx file
all : $(patsubst %.cc, %.o, $(wildcard *.cc))

# rule for each one
%.o : %.cc
	$(CXX) $(INCLUDE) $(CXXFLAGS) $(ROOTFLAGS) -o $*.o -c $*.cc #compile
	$(CXX) $(INCLUDE) $(CXXFLAGS) $(ROOTFLAGS) -o $*.exe $*.o        #link

clean:
	rm -f $(wildcard *.o) $(patsubst %.cc, %.exe, $(wildcard *.cc))
