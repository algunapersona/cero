#include "base.h"

#ifndef LONGITUD_H
#define LONGITUD_H

class Longitud: public Base {

public:

	Longitud();
	virtual ~Longitud();

	virtual void execute(int argc, char *argv[]);
};

#endif
