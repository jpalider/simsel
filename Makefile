# Makefile for Sim

CXX = ccache g++ -Wno-deprecated -Wl,--export-dynamic -std=c++11 -Wall
# -Wall
#-Wunused-function -Wall

#CXXFLAGS = -O2 `pkg-config --cflags gtk+-3.0 libconfig++` -DTLOG
CXXFLAGS = -O2 `pkg-config --cflags libconfig++` -DTLOG -DNOGUI
#-DFTLOG=test

LDFLAGS = `pkg-config --libs gtk+-3.0 libconfig++` -lm -pthread

OBJ = main.o Molecule.o Vector.o BrownianMotion.o CairoColor.o \
	Math.o \
	Simulation.o Boundary.o tri_logger/tri_logger.o \
	Boundary.o Source.o Receptor.o Obstacle.o \
	Generator.o \
	Statistics.o StatisticsDistribution.o StatisticsDensity.o
#	Test.o
#	Player.o


%.o: %.c
	$(CXX) $(CXXFLAGS) -c -o $@ $<

sim: $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)

clean:
	rm sim; find -name "*.o" | xargs rm -rf
