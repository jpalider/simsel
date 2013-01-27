# Makefile for Sim

CXX = g++ -Wno-deprecated -Wl,--export-dynamic
# -Wall
#-Wunused-function -Wall

CXXFLAGS = -O2 `pkg-config --cflags gtk+-3.0`

LDFLAGS = `pkg-config --libs gtk+-3.0`

OBJ = main.o Molecule.o Vector.o BrownianMotion.o Math.o CairoColor.o Simulation.o Cell.o

%.o: %.c
	$(CXX) $(CXXFLAGS) -c -o $@ $<

sim: $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)

clean:
	rm *.o sim
