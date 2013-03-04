# Makefile for Sim

CXX = ccache g++ -Wno-deprecated -Wl,--export-dynamic
# -Wall
#-Wunused-function -Wall

CXXFLAGS = -O2 `pkg-config --cflags gtk+-3.0 libconfig++` -DTLOG
#-DFTLOG=test

LDFLAGS = `pkg-config --libs gtk+-3.0 libconfig++` -lm

OBJ = main.o Molecule.o Vector.o BrownianMotion.o Math.o CairoColor.o \
	Simulation.o Cell.o tri_logger/tri_logger.o Player.o \
	TCell.o RCell.o \
	Statistics.o StatisticsDistribution.o StatisticsDensity.o \
	Test.o


%.o: %.c
	$(CXX) $(CXXFLAGS) -c -o $@ $<

sim: $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)

clean:
	rm sim; find -name "*.o" | xargs rm -rf
