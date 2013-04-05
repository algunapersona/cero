#include "base.h"

#ifndef PLANO_H
#define PLANO_H

class Plano: public Base {

public:

	Plano();
	virtual ~Plano();

	virtual void execute(int argc, char *argv[]);
};

#endif
