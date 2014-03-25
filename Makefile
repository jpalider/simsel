# Makefile for Sim

CXX = ccache g++ -Wno-deprecated -Wl,--export-dynamic
# -Wall
#-Wunused-function -Wall

CXXFLAGS = -O2 `pkg-config --cflags gtk+-3.0 libconfig++` -DTLOG
#-DFTLOG=test

LDFLAGS = `pkg-config --libs gtk+-3.0 libconfig++` -lm

OBJ = main.o Molecule.o Vector.o BrownianMotion.o CairoColor.o \
	Math.o SimpleRNG.o \
	Simulation.o Boundary.o tri_logger/tri_logger.o Player.o \
	Source.o Receptor.o \
	Statistics.o StatisticsDistribution.o StatisticsDensity.o \
	Test.o


%.o: %.c
	$(CXX) $(CXXFLAGS) -std++11 -c -o $@ $<

sim: $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)

clean:
	rm sim; find -name "*.o" | xargs rm -rf
