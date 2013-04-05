#include "base.h"

#ifndef ESPACIO_H
#define ESPACIO_H

class Espacio: public Base {

public:

	Espacio();
	virtual ~Espacio();

	virtual void execute(int argc, char *argv[]);
};

#endif
