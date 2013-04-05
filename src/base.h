#include <iostream>
#include <string>
#include <fstream>
#include <cstdio>

#ifndef BASE_H
#define BASE_H

using namespace std;

class Base {

public:

	Base();
	virtual ~Base();

	virtual void execute(int argc, char *argv[]) = 0;

	long tamanoArchivo(string file);
};

#endif
