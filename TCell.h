#ifndef SIM_TCELL_H
#define SIM_TCELL_H

#include "Cell.h"
#include "Molecule.h"

class TCell : public Cell
{
public:
	TCell(long identifier, Vector position, float radius);
};

#endif /* SIM_TCELL_H */
