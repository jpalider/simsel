# Makefile for Sim

CXX = ccache g++ -Wno-deprecated -Wl,--export-dynamic -std=c++11 -Wall

CXXFLAGS = -O3 `pkg-config --cflags libconfig++` -DTLOG -DNOGUI

LDFLAGS = `pkg-config --libs libconfig++` -lm -pthread -lconfig++ -lconfig++

OBJ = main.o Molecule.o Vector.o BrownianMotion.o CairoColor.o \
	Math.o \
	Simulation.o Boundary.o tri_logger/tri_logger.o \
	Boundary.o Source.o Receptor.o Obstacle.o \
	Generator.o \
	Statistics.o StatisticsDistribution.o StatisticsDensity.o \
	Test.o

%.o: %.c
	$(CXX) $(CXXFLAGS) -c -o $@ $<

sim: $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)

clean:
	rm sim; find -name "*.o" | xargs rm -rf
